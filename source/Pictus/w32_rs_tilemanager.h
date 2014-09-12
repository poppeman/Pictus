#ifndef W32_RS_TILEMANAGER_H
#define W32_RS_TILEMANAGER_H

#include "w32_ddsurface.h"
#include "w32_renderer.h"
#include "illa/types.h"

namespace Win {
	struct TileManager {
		enum {
			MaximumTileEdgeLength = 512,
		};

		void AddOffset(const Geom::SizeInt& pt);
		void SetViewportSize(const Geom::SizeInt& dims);

		struct RequestedArea {
			DDSurface::Ptr Surface;
			Geom::RectInt WriteableArea;
		};
		RequestedArea RequestDDSurface(const Geom::RectInt& areaToRequest) const;

		void Render(Geom::SizeInt offset);

		void RenderTiles(Geom::SizeInt offset, bool renderAll);
		TileManager(Renderer::Ptr device);

		typedef std::shared_ptr<TileManager> Ptr;

	private:
		Geom::RectInt determineLockableRect(const Geom::RectInt& r) const;
		Geom::PointInt determineTileCoords(const Geom::PointInt& p) const;

		Renderer::Ptr m_device;
		struct Tile {
			DDSurface::Ptr surface;
			Geom::RectInt dirtyRect;
		};

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
