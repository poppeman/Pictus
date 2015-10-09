#include "w32_renderer.h"
#include "illa/geom.h"
#include "D3DWrap/d3d_math.h"
#include "illa/swsurface.h"
#include "d3d_ddsurface.h"
#include "illa/render.h"

#include <algorithm>

namespace Win {
	using namespace Geom;

	Geom::SizeInt Renderer::RenderAreaSize() {
		RECT cr;
		GetClientRect(TargetWindow(), &cr);
		return{ cr.right - cr.left, cr.bottom - cr.top };
	}

	Geom::SizeInt Renderer::TransformedRenderAreaSize() {
		return Transform(RenderAreaSize());
	}


	Geom::SizeInt Renderer::Transform(Geom::SizeInt sz) {
		switch (Angle) {
			case Filter::RotationAngle::Rotate90:
			case Filter::RotationAngle::Rotate270:
				return{ sz.Height, sz.Width };
		}
		return sz;
	}

	Geom::PointInt Renderer::TransformPan(Geom::PointInt sz, Geom::SizeInt imageSize) {
		auto ras = RenderAreaSize();
		switch (Angle) {
			case Filter::RotationAngle::FlipX:
				return{ std::max(0, (imageSize.Width - ras.Width) - sz.X), sz.Y };

			case Filter::RotationAngle::FlipY:
				return{ sz.X, std::max(0, (imageSize.Height - ras.Height) - sz.Y) };

			case Filter::RotationAngle::Rotate90:
				return{ sz.Y, std::max(0, (imageSize.Height - ras.Width) - sz.X) };

			case Filter::RotationAngle::Rotate180:
				return{ std::max(0, (imageSize.Width - ras.Width) - sz.X), std::max(0, (imageSize.Height - ras.Height) - sz.Y) };

			case Filter::RotationAngle::Rotate270:
				return{ std::max(0, (imageSize.Width - ras.Height) - sz.Y), sz.X };
		}
		return sz;
	}

	bool Renderer::TargetWindow( HWND hwnd ) {
		m_hwnd = hwnd;

		if (m_direct3d == nullptr) {
			m_direct3d = std::make_shared<D3D::Device>();
		}

		if (m_direct3d->Initialize(TargetWindow()) == false) {
			m_direct3d.reset();
			return false;
		}
		return true;

	}

	Renderer::RenderStatus Renderer::BeginRender(Img::Color backgroundColor) {
		static float a = 0.0f;
		if (m_hwnd == nullptr) {
			DO_THROW(Err::CriticalError, "Target window not set.");
		}

		if (m_direct3d == nullptr) {
			DO_THROW(Err::CriticalError, "Direct3D not yet initialized.");
		}

		if (m_direct3d->IsLost()) {
			m_direct3d->Reset();
		}

		// If the device still is lost, some resources (some possibly outside of this object) are being held that must be released.
		// Return so any external objects can do the required cleanup before attempting again.
		if (m_direct3d->IsLost()) {
			m_softTex.reset();
			return RenderStatus::CurrentViewLost;
		}

		CreateTextures();

		m_direct3d->BeginDraw();
		m_direct3d->Clear(0xff, backgroundColor.R, backgroundColor.G, backgroundColor.B);

		auto proj = D3D::OrthographicProjection({ { 0, 0 }, RenderAreaSize().StaticCast<float>() });
		m_direct3d->SetMatrix(D3DTS_PROJECTION, &proj);

		return RenderStatus::OK;
	}

	void Renderer::EndRender() {
		if (m_hwnd == nullptr) {
			DO_THROW(Err::CriticalError, "Target window not set.");
		}
		if (m_direct3d == nullptr) {
			DO_THROW(Err::CriticalError, "Direct3D not yet initialized.");
		}

		m_direct3d->EndDraw();
	}

	Renderer::Renderer():m_hwnd(0) {}
	Renderer::~Renderer() {}

	HWND Renderer::TargetWindow() { return m_hwnd; }

	DDSurface::Ptr Renderer::CreateDDSurface() {
		return std::make_shared<DDSurfaceD3D>(m_direct3d);
	}

	void Renderer::CreateTextures() {
		if (m_direct3d->BackBufferSize() != RenderAreaSize()) {
			m_direct3d->ResizeBackBuffer(RenderAreaSize());
		}
	}

	Img::Surface::Ptr Renderer::CreateSurface() {
		return std::make_shared<Img::SurfaceSoftware>();
	}

	void Renderer::RenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props) {
		DDSurfaceD3D* ds = dynamic_cast<DDSurfaceD3D*>(dest.get());

		if (m_softTex == 0 || !m_softTex->GetSize().AtLeastInclusive(destinationArea.Dimensions())) {
			m_softTex = m_direct3d->CreateTexture(destinationArea.Dimensions(), D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM);
		}

		D3D::Texture::Lock l = m_softTex->LockRegion(RectInt(PointInt(0, 0), destinationArea.Dimensions()), false);
		try {
			Filter::FilterBuffer dst(m_softTex->GetSize(), 4, l.Buffer, l.Pitch);

			Img::FilterBufferAndLock src = GenerateFilterBuffer(source);
			if (src.lock == nullptr) {
				m_softTex->UnlockRegion();
				return;
			}

			Img::RenderToBuffer(dst, src.filterBuffer, source->GetFormat(), RectInt(zoomedImagePosition, destinationArea.Dimensions()), props);

			m_softTex->UnlockRegion();

			m_direct3d->SendTextureRect(
				m_softTex,
				RectInt(PointInt(0, 0),
				destinationArea.Dimensions()),
				ds->GetTexture(),
				destinationArea.TopLeft());
		}
		catch (...) {
			m_softTex->UnlockRegion();
			throw;
		}
	}

	void Renderer::PresentFromDDSurface(Geom::RectInt destRect, DDSurface::Ptr source, Geom::PointInt sourceTopLeft) {
		SizeFloat ppAdj{ -0.5f, -0.5f };

		auto* ds = dynamic_cast<DDSurfaceD3D*>(source.get());
		auto tex = ds->GetTexture();
		m_direct3d->SetTexture(0, tex);

		D3D::Vertex2D a, b, c, d;
		auto uvTL = sourceTopLeft.StaticCast<float>() / source->Dimensions().StaticCast<float>();
		auto uvBR = (sourceTopLeft + destRect.Dimensions()).StaticCast<float>() / source->Dimensions().StaticCast<float>();
		D3D::GenerateQuad(
			destRect.StaticCast<float>(),
			{ uvTL, uvBR },
			ppAdj,
			RenderAreaSize(),
			Angle,
			a, b, c, d);
		m_direct3d->RenderQuad(a, b, c, d);
	}

}
