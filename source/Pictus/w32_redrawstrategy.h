#ifndef W32_REDRAWSTRATEGY_H
#define W32_REDRAWSTRATEGY_H

#include "w32_renderer.h"

namespace Win {
	class RedrawStrategy {
	public:
		void Render(Renderer::Ptr newRenderer, Img::Surface::Ptr surfaceToRender, const Geom::PointInt& pan, const Img::Properties& props);
		void Reset();

		virtual ~RedrawStrategy();

		typedef std::shared_ptr<RedrawStrategy> Ptr;

	private:
		virtual void OnRender(Renderer::Ptr renderer, Img::Surface::Ptr surfaceToRender, const Geom::PointInt& pan, const Img::Properties& props)=0;
		virtual void OnReset()=0;
	};
}

#endif
