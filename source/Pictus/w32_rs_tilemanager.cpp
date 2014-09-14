#include "StdAfx.h"
#include "w32_rs_tilemanager.h"

namespace Win {
	using namespace Geom;

	void TileManager::AddOffset( const Geom::SizeInt& pt ) {
		m_panDelta += pt;

		m_topLeftOffset += -pt;
		SizeInt ourOffset = m_topLeftOffset;
		if (ourOffset.Width < 0) ourOffset.Width -= MaximumTileEdgeLength - 1;
		if (ourOffset.Height < 0) ourOffset.Height -= MaximumTileEdgeLength - 1;
		auto newTlIndex = PointInt(0, 0) + ourOffset / MaximumTileEdgeLength;

		for(int y = 0; y < newTlIndex.Y; ++y) {
			auto r = m_tiles.front();
			m_tiles.pop_front();
			m_tiles.push_back(r);
		}
		for(int y = 0; y < -newTlIndex.Y; ++y) {
			auto r = m_tiles.back();
			m_tiles.pop_back();
			m_tiles.push_front(r);
		}

		while (m_topLeftOffset.Height >= MaximumTileEdgeLength) {
			m_topLeftOffset += SizeInt(0, -MaximumTileEdgeLength);
		}
		while (m_topLeftOffset.Height < 0) {
			m_topLeftOffset += SizeInt(0, MaximumTileEdgeLength);
		}

		for (int x = 0; x < newTlIndex.X; ++x) {
			for (size_t y = 0; y < m_tiles.size(); ++y) {
				Tile t = m_tiles[y]->front();
				m_tiles[y]->pop_front();
				m_tiles[y]->push_back(t);
			}
		}

		for (int x = 0; x < -newTlIndex.X; ++x) {
			for (size_t y = 0; y < m_tiles.size(); ++y) {
				Tile t = m_tiles[y]->back();
				m_tiles[y]->pop_back();
				m_tiles[y]->push_front(t);
			}
		}

		while (m_topLeftOffset.Width >= MaximumTileEdgeLength) m_topLeftOffset += SizeInt(-MaximumTileEdgeLength, 0);
		while (m_topLeftOffset.Width < 0) m_topLeftOffset += SizeInt(MaximumTileEdgeLength, 0);
	}

	void TileManager::SetViewportSize( const SizeInt& dims ) {
		if (dims.Width < 0 || dims.Height < 0) {
			DO_THROW(Err::InvalidParam, TX("Invalid dimensions: ") + ToWString(dims.Width) + TX(", ") + ToWString(dims.Height));
		}
		const SizeInt texDims = SizeInt(MaximumTileEdgeLength, MaximumTileEdgeLength);
		const SizeInt numTiles = SizeInt(2, 2) + dims / MaximumTileEdgeLength;

		m_viewSize = dims;
		m_tiles.resize(numTiles.Height);

		for(auto& r: m_tiles) {
			if (r == 0) {
				r.reset(new TileRow);
			}
			r->resize(numTiles.Width);

			for(auto& t: *r) {
				if(t.surface == 0) {
					t.surface = m_device->CreateDDSurface();
					if (t.surface == nullptr) {
						DO_THROW(Err::CriticalError, TX("Failed to create surface."));
					}
					t.surface->Create(texDims);
				}
			}
		}
	}

	TileManager::TileManager(Renderer::Ptr device):m_device(device) {
		if (device == nullptr) {
			DO_THROW(Err::InvalidParam, TX("Device may not be null."));
		}
	}

	void TileManager::Render(Geom::SizeInt offset) {
		if (m_topLeftOffset.Width < 0) {
			DO_THROW(Err::CriticalError, TX("Offset (X) should always be positive"));
		}
		if (m_topLeftOffset.Height < 0) {
			DO_THROW(Err::CriticalError, TX("Offset (Y) should always be positive"));
		}

		if ((m_panDelta.Width != 0 || m_panDelta.Height != 0)) {
			RenderTiles(offset, !m_device->PanCurrentView(m_panDelta));
		}
		else {
			RenderTiles(offset, false);
		}

		m_panDelta = SizeInt(0, 0);
	}

	Geom::RectInt TileManager::determineLockableRect( const RectInt& r ) const {
		COND_STRICT(IsPositive(r.Dimensions()), Err::InvalidParam, TX("Invalid dimensions"));
		COND_STRICT(m_topLeftOffset.AtLeastInclusive(SizeInt(0, 0)), Err::CriticalError, TX("Invalid offset"));

		PointInt pannedTopLeft = r.TopLeft() + m_topLeftOffset;
		while(pannedTopLeft.X >= MaximumTileEdgeLength) pannedTopLeft.X -= MaximumTileEdgeLength;
		while(pannedTopLeft.Y >= MaximumTileEdgeLength) pannedTopLeft.Y -= MaximumTileEdgeLength;

		PointInt croppedBottomRight = Geom::Minimum(r.BottomRight(), PointInt(0, 0) + m_topLeftOffset + m_viewSize);
		SizeInt outSize = Maximum(SizeInt(0, 0), 
			Minimum(
				SizeInt(MaximumTileEdgeLength, MaximumTileEdgeLength) + (PointInt(0, 0) - pannedTopLeft),
				croppedBottomRight - PointInt(0, 0),
				r.Dimensions()));

		COND_STRICT(IsPositive(outSize), Err::CriticalError, TX("Calculation error, rect became empty."));
		return RectInt(pannedTopLeft, outSize);
	}

	Geom::PointInt TileManager::determineTileCoords( const Geom::PointInt& p ) const {
		COND_STRICT(m_topLeftOffset.AtLeastInclusive(SizeInt(0, 0)), Err::CriticalError, TX("Invalid offset"));

		PointInt toRet = (p + m_topLeftOffset) / MaximumTileEdgeLength;
		COND_STRICT(toRet.Y < static_cast<int>(m_tiles.size()) && toRet.X < static_cast<int>(m_tiles[toRet.Y]->size()),
			Err::InvalidParam, TX("Tile coordinates out of bounds: ") + ToWString(toRet));
		return toRet;
	}

	TileManager::RequestedArea TileManager::RequestDDSurface(const Geom::RectInt& areaToRequest) const {
		COND_STRICT(IsPositive(areaToRequest.Dimensions()), Err::InvalidParam, TX("Zero or negative size."));

		Geom::PointInt tileCoords= determineTileCoords(areaToRequest.TopLeft());

		RequestedArea ra;
		ra.WriteableArea = determineLockableRect(areaToRequest);

		COND_STRICT(IsPositive(ra.WriteableArea.Dimensions()), Err::CriticalError, TX("Invalid writeable area."));
		COND_STRICT(ra.WriteableArea.TopLeft().AtLeastInclusive(PointInt(0, 0)), Err::CriticalError, TX("Invalid top-left of writable area"));

		Tile& t = (*m_tiles[tileCoords.Y])[tileCoords.X];
		if(!IsPositive(t.dirtyRect.Dimensions())) {
			t.dirtyRect = ra.WriteableArea;
		}
		else {
			t.dirtyRect.TopLeft(Minimum(t.dirtyRect.TopLeft(), ra.WriteableArea.TopLeft()));
			t.dirtyRect.BottomRight(Maximum(t.dirtyRect.BottomRight(), ra.WriteableArea.BottomRight()));
		}
		COND_STRICT(t.dirtyRect.TopLeft().AtLeastInclusive(PointInt(0, 0)),
			Err::CriticalError, TX("Dirty rect became invalid (TL too small)."));
		COND_STRICT(t.dirtyRect.TopLeft().AtMostExclusive(PointInt(MaximumTileEdgeLength, MaximumTileEdgeLength)),
			Err::CriticalError, TX("Dirty rect became invalid (TL too large)."));
		COND_STRICT(t.dirtyRect.Dimensions().AtLeastInclusive(SizeInt(0, 0)),
			Err::CriticalError, TX("Dirty rect became invalid (negative size)."));
		ra.Surface = t.surface;
		return ra;
	}

	void TileManager::RenderTiles(Geom::SizeInt offset, bool renderAll) {
		if(m_tiles.empty()) return;

		auto xtiles = m_tiles[0]->size();
		auto ytiles = m_tiles.size();
		RectInt view{ { 0, 0 }, m_viewSize };

		auto ppAdj = SizeFloat{ -0.5f, -0.5f };
		for (size_t y = 0; y < ytiles; ++y) {
			for (size_t x = 0; x < xtiles; ++x) {
				auto currUncropped = RectInt{ PointInt( x, y ) * MaximumTileEdgeLength, SizeInt(1, 1) * MaximumTileEdgeLength };
				SizeInt tlDelta;
				SizeInt srcDelta;
				if(!renderAll) {
					RectInt dirtyRect = (*m_tiles[y])[x].dirtyRect;
					if (!IsPositive(dirtyRect.Dimensions())) {
						continue;
					}
					tlDelta = dirtyRect.TopLeft() - PointInt(0, 0);
					srcDelta = tlDelta;
					currUncropped.Dimensions(Minimum(currUncropped.Dimensions(), dirtyRect.Dimensions()));
				}

				(*m_tiles[y])[x].dirtyRect.Dimensions(SizeInt(0, 0));
				currUncropped.TopLeft(currUncropped.TopLeft() - m_topLeftOffset + tlDelta);

				auto currCropped = view.Crop(currUncropped);

				if (renderAll) {
					srcDelta = currCropped.TopLeft() - currUncropped.TopLeft();
				}

				if (!IsPositive(currCropped.Dimensions())) {
					continue;
				}
				if (!view.Contains(currCropped.TopLeft())) {
					continue;
				}

				currCropped.TopLeft(currCropped.TopLeft() + offset);
				m_device->PresentFromDDSurface(
					currCropped,
					(*m_tiles[y])[x].surface,
					Maximum(PointInt(0, 0) + srcDelta, PointInt(0, 0)));
			}
		}
	}
}
