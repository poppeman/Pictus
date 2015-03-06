#ifndef W32_RS_TILED_H
#define W32_RS_TILED_H

#include "w32_redrawstrategy.h"
#include "w32_rs_tilemanager.h"

namespace Win {
	class RedrawStrategyTiled:public RedrawStrategy {
	public:
		RedrawStrategyTiled();

	private:
		void OnReset() override;
		void OnRender(Renderer::Ptr renderer, Img::Surface::Ptr surfaceToRender, Geom::PointInt pan, Img::Properties props) override;

		void RenderArea(Renderer::Ptr renderer, Img::Surface::Ptr surface, Geom::PointInt zoomedImagePosition, Geom::RectInt destinationArea, Img::Properties props);

		bool m_redrawNext;
		Geom::PointInt m_currPosition;
		Geom::SizeInt m_currentSize;

		Img::Properties m_prevProperties;
		Img::Surface* m_prevSurface;

		TileManager::Ptr m_tiles;
	};
}

#endif
