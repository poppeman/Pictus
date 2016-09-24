#ifndef W32_RENDERTARGET_H
#define W32_RENDERTARGET_H

#include "illa/surfacefactory.h"

#include "w32_renderer.h"
#include "w32_redrawstrategy.h"

namespace Win {
	class WinRenderTarget:public Img::ISurfaceFactory {
	public:
		void SetRenderer(Renderer::Ptr newRenderer);
		void SetRedrawStrategy(RedrawStrategy::Ptr newRedrawStrategy);

		//Geom::SizeInt CurrentSurfaceSizeUnzoomed() const;
		Img::Surface::Ptr CurrentSurface() const;
		void Render(const Geom::PointInt& sourceZoomedTopLeft, const Img::Properties& props);
		void Clear(Img::Color backgroundColor);
		void SetSurface(Img::Surface::Ptr surface);
		bool TargetWindow(wxWindow* hwnd);

		Geom::SizeInt TransformedSize() const;

		Img::Surface::Ptr CreateSurface();

		WinRenderTarget();

	private:
		Renderer::Ptr m_renderer;
		RedrawStrategy::Ptr m_redrawStrategy;

		wxWindow* m_hwnd;
		Img::Surface::Ptr m_surface;
	};
}

#endif
