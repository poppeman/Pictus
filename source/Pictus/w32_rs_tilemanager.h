#ifndef PICTUS_W32_RS_TILEMANAGER_H
#define PICTUS_W32_RS_TILEMANAGER_H

#include "illa/types.h"

#include "w32_renderer.h"

#include <deque>
#include <memory>

namespace Win {
	struct TileManager {
		enum {
			MaximumTileEdgeLength = 512,
		};

		void AddOffset(const Geom::SizeInt& pt);
		void SetViewportSize(const Geom::SizeInt& dims);

		struct RequestedArea {
			std::shared_ptr<Hw3D::Texture> Surface;
			Geom::RectInt WriteableArea;
		};
		RequestedArea RequestDDSurface(const Geom::RectInt& areaToRequest) const;

		void Render(Geom::SizeInt offset);

		TileManager(Renderer::Ptr device);

		typedef std::shared_ptr<TileManager> Ptr;

	private:
		struct Tile {
			std::shared_ptr<Hw3D::Texture> surface;
			Geom::RectInt dirtyRect;
		};

		Geom::RectInt determineLockableRect(const Geom::RectInt& r) const;
		Geom::PointInt determineTileCoords(const Geom::PointInt& p) const;

		Renderer::Ptr m_device;

		typedef std::deque<Tile> TileRow;
		typedef std::shared_ptr<TileRow> TileRowPtr;
		typedef std::deque<TileRowPtr> TileList;
		TileList m_tiles;

		Geom::SizeInt m_topLeftOffset;
		Geom::SizeInt m_panDelta;
		Geom::SizeInt m_viewSize;
		Geom::SizeInt m_prevDims;
	};
}

#endif
