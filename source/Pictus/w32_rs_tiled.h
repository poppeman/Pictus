#ifndef W32_RS_TILED_H
#define W32_RS_TILED_H

#include "w32_redrawstrategy.h"
#include "w32_rs_tilemanager.h"

namespace Win {
	class RedrawStrategyTiled:public RedrawStrategy {
	public:
		RedrawStrategyTiled();

	private:
		void OnReset();
		void OnRender(Renderer::Ptr renderer, Img::Surface::Ptr surfaceToRender, const Geom::PointInt& pan, const Img::Properties& props) override;

		void RenderArea(Renderer::Ptr renderer, Img::Surface::Ptr surface, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, Img::Properties props);
		void TouchTiles(const Geom::RectInt& destinationArea);

		bool m_redrawNext;
		Geom::PointInt m_currPosition;
		Geom::SizeInt m_currentSize;

		Img::Properties m_prevProperties;
		Img::Surface* m_prevSurface;

		TileManager::Ptr m_tiles;
	};
}

#endif
