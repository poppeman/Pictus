#include "StdAfx.h"
#include "w32_renderer.h"
#include "illa/geom.h"

namespace Win {
	bool Renderer::TargetWindow( HWND hwnd ) {
		m_hwnd = hwnd;
		return OnTargetWindowChanged();
	}

	Renderer::RenderStatus Renderer::BeginRender() {
		if (m_hwnd == 0) DO_THROW(Err::CriticalError, TX("Target window not set."));
		return OnBeginRender();
	}

	void Renderer::EndRender() {
		if (m_hwnd == 0) DO_THROW(Err::CriticalError, TX("Target window not set."));
		OnEndRender();
	}

	Renderer::Renderer():m_hwnd(0) {}
	Renderer::~Renderer() {}

	HWND Renderer::TargetWindow() { return m_hwnd; }
	bool Renderer::OnTargetWindowChanged() { return true; }
	Renderer::RenderStatus Renderer::OnBeginRender() { return Renderer::RS_OK; }
	void Renderer::OnEndRender() {}

	void Renderer::RenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props) {
		OnRenderToDDSurface(dest, source, zoomedImagePosition, destinationArea,  props);
	}

	void Renderer::PresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft) {
		OnPresentFromDDSurface(destRect, source, sourceTopLeft);
	}

	DDSurface::Ptr Renderer::CreateDDSurface() {
		DDSurface::Ptr s = OnCreateDDSurface();
		COND_STRICT(s, Err::CriticalError, TX("Failed to create surface."));
		return s;
	}

	bool Renderer::PanCurrentView(const Geom::SizeInt& ) {
		return false;
	}

	Geom::RectInt Renderer::GetInvalidArea() {
		using namespace Geom;
		return RectInt(PointInt(0, 0), SizeInt(0, 0));
	}
}
