#include "StdAfx.h"
#include "w32_rs_tilemanager.h"

namespace Win {
	using namespace Geom;

	void TileManager::AddOffset(const Geom::SizeInt& pt) {
		m_panDelta += pt;

		m_topLeftOffset += -pt;
		auto ourOffset = m_topLeftOffset;
		if (ourOffset.Width < 0) {
			ourOffset.Width -= MaximumTileEdgeLength - 1;
		}
		if (ourOffset.Height < 0) {
			ourOffset.Height -= MaximumTileEdgeLength - 1;
		}
		auto newTlIndex = PointInt(0, 0) + ourOffset / MaximumTileEdgeLength;

		for (int y = 0; y < newTlIndex.Y; ++y) {
			auto r = m_tiles.front();
			m_tiles.pop_front();
			m_tiles.push_back(r);
		}

		for (int y = 0; y < -newTlIndex.Y; ++y) {
			auto r = m_tiles.back();
			m_tiles.pop_back();
			m_tiles.push_front(r);
		}

		while (m_topLeftOffset.Height >= MaximumTileEdgeLength) {
			m_topLeftOffset += {0, -MaximumTileEdgeLength};
		}

		while (m_topLeftOffset.Height < 0) {
			m_topLeftOffset += {0, MaximumTileEdgeLength};
		}

		for (int x = 0; x < newTlIndex.X; ++x) {
			for (size_t y = 0; y < m_tiles.size(); ++y) {
				auto t = m_tiles[y]->front();
				m_tiles[y]->pop_front();
				m_tiles[y]->push_back(t);
			}
		}

		for (int x = 0; x < -newTlIndex.X; ++x) {
			for (size_t y = 0; y < m_tiles.size(); ++y) {
				auto t = m_tiles[y]->back();
				m_tiles[y]->pop_back();
				m_tiles[y]->push_front(t);
			}
		}

		while (m_topLeftOffset.Width >= MaximumTileEdgeLength) {
			m_topLeftOffset += SizeInt(-MaximumTileEdgeLength, 0);
		}
		while (m_topLeftOffset.Width < 0) {
			m_topLeftOffset += SizeInt(MaximumTileEdgeLength, 0);
		}
	}

	void TileManager::SetViewportSize(const SizeInt& dims) {
		if (dims.Width < 0 || dims.Height < 0) {
			DO_THROW(Err::InvalidParam, TX("Invalid dimensions: ") + ToWString(dims.Width) + TX(", ") + ToWString(dims.Height));
		}
		const SizeInt texDims{ MaximumTileEdgeLength, MaximumTileEdgeLength };
		const SizeInt numTiles = SizeInt(2, 2) + dims / MaximumTileEdgeLength;

		m_viewSize = dims;
		m_tiles.resize(numTiles.Height);

		for (auto& r : m_tiles) {
			if (r == nullptr) {
				r = std::make_shared<TileRow>();
			}
			r->resize(numTiles.Width);

			for (auto& t : *r) {
				if (t.surface == 0) {
					t.surface = m_device->CreateDDSurface();
					if (t.surface == nullptr) {
						DO_THROW(Err::CriticalError, TX("Failed to create surface."));
					}
					t.surface->Create(texDims);
				}
			}
		}
	}

	TileManager::TileManager(Renderer::Ptr device) :m_device(device) {
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

		if (m_tiles.empty()) {
			return;
		}

		auto xtiles = m_tiles[0]->size();
		auto ytiles = m_tiles.size();
		RectInt view{ { 0, 0 }, m_viewSize };

		auto ppAdj = SizeFloat{ -0.5f, -0.5f };
		for (size_t y = 0; y < ytiles; ++y) {
			for (size_t x = 0; x < xtiles; ++x) {
				auto currUncropped = RectInt{ PointInt(x, y) * MaximumTileEdgeLength, SizeInt(1, 1) * MaximumTileEdgeLength };
				SizeInt tlDelta;
				SizeInt srcDelta;

				auto& tile = (*m_tiles[y])[x];

				tile.dirtyRect.Dimensions({ 0, 0 });
				currUncropped.TopLeft(currUncropped.TopLeft() - m_topLeftOffset + tlDelta);

				auto currCropped = view.Crop(currUncropped);
				srcDelta = currCropped.TopLeft() - currUncropped.TopLeft();

				if (!IsPositive(currCropped.Dimensions())) {
					continue;
				}
				if (!view.Contains(currCropped.TopLeft())) {
					continue;
				}

				currCropped.TopLeft(currCropped.TopLeft() + offset);
				m_device->PresentFromDDSurface(
					currCropped,
					tile.surface,
					Maximum(PointInt(0, 0) + srcDelta, PointInt(0, 0)));
			}
		}

		m_panDelta = { 0, 0 };
	}

	Geom::RectInt TileManager::determineLockableRect(const RectInt& r) const {
		if (IsPositive(r.Dimensions()) == false) {
			DO_THROW(Err::InvalidParam, L"Invalid dimensions");
		}

		if (m_topLeftOffset.AtLeastInclusive({ 0, 0 }) == false) {
			DO_THROW(Err::CriticalError, L"Invalid offset");
		}

		auto pannedTopLeft = r.TopLeft() + m_topLeftOffset;
		while (pannedTopLeft.X >= MaximumTileEdgeLength) {
			pannedTopLeft.X -= MaximumTileEdgeLength;
		}
		while (pannedTopLeft.Y >= MaximumTileEdgeLength) {
			pannedTopLeft.Y -= MaximumTileEdgeLength;
		}

		auto croppedBottomRight = Geom::Minimum(r.BottomRight(), PointInt(0, 0) + m_topLeftOffset + m_viewSize);
		auto outSize = Maximum(SizeInt(0, 0),
			Minimum(
			SizeInt(MaximumTileEdgeLength, MaximumTileEdgeLength) + (PointInt(0, 0) - pannedTopLeft),
			croppedBottomRight - PointInt(0, 0),
			r.Dimensions()));

		if (IsPositive(outSize) == false) {
			DO_THROW(Err::CriticalError, L"Calculation error, rect became empty.");
		}
		return{ pannedTopLeft, outSize };
	}

	Geom::PointInt TileManager::determineTileCoords(const Geom::PointInt& p) const {
		if (m_topLeftOffset.AtLeastInclusive({ 0, 0 }) == false) {
			DO_THROW(Err::CriticalError, L"Invalid offset");
		}

		PointInt toRet = (p + m_topLeftOffset) / MaximumTileEdgeLength;
		if (toRet.Y >= static_cast<int>(m_tiles.size()) || toRet.X >= static_cast<int>(m_tiles[toRet.Y]->size())) {
			DO_THROW(Err::InvalidParam, L"Tile coordinates out of bounds: " + ToWString(toRet));
		}
		return toRet;
	}

	TileManager::RequestedArea TileManager::RequestDDSurface(const Geom::RectInt& areaToRequest) const {
		if (IsPositive(areaToRequest.Dimensions()) == false) {
			DO_THROW(Err::InvalidParam, TX("Zero or negative size."));
		}

		auto tileCoords = determineTileCoords(areaToRequest.TopLeft());

		RequestedArea ra;
		ra.WriteableArea = determineLockableRect(areaToRequest);

		if (IsPositive(ra.WriteableArea.Dimensions()) == false) {
			DO_THROW(Err::CriticalError, L"Invalid writeable area.");
		}
		if (ra.WriteableArea.TopLeft().AtLeastInclusive({ 0, 0 }) == false) {
			DO_THROW(Err::CriticalError, TX("Invalid top-left of writable area"));
		}

		Tile& t = (*m_tiles[tileCoords.Y])[tileCoords.X];
		if (!IsPositive(t.dirtyRect.Dimensions())) {
			t.dirtyRect = ra.WriteableArea;
		}
		else {
			t.dirtyRect.TopLeft(Minimum(t.dirtyRect.TopLeft(), ra.WriteableArea.TopLeft()));
			t.dirtyRect.BottomRight(Maximum(t.dirtyRect.BottomRight(), ra.WriteableArea.BottomRight()));
		}
		if (t.dirtyRect.TopLeft().AtLeastInclusive({ 0, 0 }) == false) {
			DO_THROW(Err::CriticalError, L"Dirty rect became invalid (TL too small).");
		}
		if (t.dirtyRect.TopLeft().AtMostExclusive({ MaximumTileEdgeLength, MaximumTileEdgeLength }) == false) {
			DO_THROW(Err::CriticalError, L"Dirty rect became invalid (TL too large).");
		}
		if (t.dirtyRect.Dimensions().AtLeastInclusive({ 0, 0 }) == false) {
			DO_THROW(Err::CriticalError, L"Dirty rect became invalid (negative size).");
		}
		ra.Surface = t.surface;
		return ra;
	}
}
