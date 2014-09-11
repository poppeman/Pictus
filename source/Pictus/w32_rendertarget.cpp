#include "StdAfx.h"
#include "w32_rendertarget.h"
#include "illa/config.h"

namespace Win {
	void WinRenderTarget::SetRenderer( Renderer::Ptr newRenderer ) {
		m_renderer = newRenderer;
	}

	void WinRenderTarget::SetRedrawStrategy( RedrawStrategy::Ptr newRedrawStrategy ) {
		m_redrawStrategy = newRedrawStrategy;
	}

	WinRenderTarget::WinRenderTarget()
		:m_hwnd(0)
	{}

	bool WinRenderTarget::TargetWindow( HWND hwnd ) {
		if (m_renderer == nullptr) {
			return false;
		}

		m_hwnd = hwnd;
		return m_renderer->TargetWindow(hwnd);
	}

	void WinRenderTarget::Render( const Geom::PointInt& sourceZoomedTopLeft, const Img::Properties& props ) {
		if (m_hwnd == nullptr) {
			DO_THROW(Err::CriticalError, TX("TargetWindow not set"));
		}
		if (m_renderer == nullptr) {
			DO_THROW(Err::CriticalError, TX("Renderer not set"));
		}
		if (m_redrawStrategy == nullptr) {
			DO_THROW(Err::CriticalError, TX("Redraw strategy not set"));
		}

		m_redrawStrategy->Render(m_renderer, m_surface, sourceZoomedTopLeft, props);
	}

	void WinRenderTarget::SetSurface( Img::Surface::Ptr surface ) {
		m_surface = surface;
	}

	Img::Surface::Ptr WinRenderTarget::CurrentSurface() const {
		return m_surface;
	}

	Img::Surface::Ptr WinRenderTarget::CreateSurface() {
		if (m_renderer == nullptr) {
			DO_THROW(Err::CriticalError, TX("Renderer not yet set."));
		}
		return m_renderer->CreateSurface();
	}
}
