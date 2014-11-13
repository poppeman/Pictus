#include "StdAfx.h"
#include "w32_renderer.h"
#include "illa/geom.h"

namespace Win {
	bool Renderer::TargetWindow( HWND hwnd ) {
		m_hwnd = hwnd;
		return OnTargetWindowChanged();
	}

	Renderer::RenderStatus Renderer::BeginRender(Img::Color backgroundColor) {
		if (m_hwnd == nullptr) {
			DO_THROW(Err::CriticalError, TX("Target window not set."));
		}

		return OnBeginRender(backgroundColor);
	}

	void Renderer::EndRender() {
		if (m_hwnd == nullptr) {
			DO_THROW(Err::CriticalError, TX("Target window not set."));
		}
		OnEndRender();
	}

	Renderer::Renderer():m_hwnd(0) {}
	Renderer::~Renderer() {}

	HWND Renderer::TargetWindow() { return m_hwnd; }
	bool Renderer::OnTargetWindowChanged() { return true; }

	void Renderer::OnEndRender() {}

	void Renderer::RenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props) {
		OnRenderToDDSurface(dest, source, zoomedImagePosition, destinationArea,  props);
	}

	void Renderer::PresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft, Filter::RotationAngle angle) {
		OnPresentFromDDSurface(destRect, source, sourceTopLeft, angle);
	}

	DDSurface::Ptr Renderer::CreateDDSurface() {
		DDSurface::Ptr s = OnCreateDDSurface();
		if (s == nullptr) {
			DO_THROW(Err::CriticalError, TX("Failed to create surface."));
		}
		return s;
	}

	bool Renderer::PanCurrentView(const Geom::SizeInt& ) {
		return false;
	}
}
