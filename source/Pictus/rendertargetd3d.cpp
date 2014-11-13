#include "StdAfx.h"

#include "rendertargetd3d.h"

#include "illa/swsurface.h"
#include "d3d_ddsurface.h"
#include "D3DWrap/d3d_math.h"

#include "illa/render.h"

namespace Win {
	using namespace Geom;

	Img::Surface::Ptr RendererD3D::CreateSurface() {
		return std::make_shared<Img::SurfaceSoftware>();
	}

	SizeInt RendererD3D::RenderAreaSize() {
		RECT cr;
		GetClientRect(TargetWindow(), &cr);
		return{ cr.right - cr.left, cr.bottom - cr.top };
	}

	bool RendererD3D::OnTargetWindowChanged() {
		if (m_direct3d == nullptr) {
			m_direct3d = std::make_shared<D3D::Device>();
		}

		if (m_direct3d->Initialize(TargetWindow()) == false) {
			m_direct3d.reset();
			return false;
		}
		return true;
	}

	Renderer::RenderStatus RendererD3D::OnBeginRender(Img::Color backgroundColor) {
		if (m_direct3d == nullptr) {
			DO_THROW(Err::CriticalError, TX("Direct3D not yet initialized."));
		}

		if (m_direct3d->IsLost()) {
			m_direct3d->Reset();
		}

		// If the device still is lost, some resources (some possibly outside of this object) are being held that must be released.
		// Return so any external objects can do the required cleanup before attempting again.
		if (m_direct3d->IsLost()) {
			m_swapChain.reset();
			m_softTex.reset();
			return RS_CurrentViewLost;
		}

		CreateTextures();

		m_direct3d->BeginDraw();
		m_direct3d->SetSwapChain(m_swapChain);
		m_direct3d->Clear(0xff, backgroundColor.R, backgroundColor.G, backgroundColor.B);

		return RS_OK;
	}

	void RendererD3D::OnEndRender() {
		if (m_direct3d == nullptr) {
			DO_THROW(Err::CriticalError, TX("Direct3D not yet initialized."));
		}

		m_direct3d->EndDraw();
		m_swapChain->Present();
	}

	void RendererD3D::CreateTextures() {
		if ((m_swapChain == nullptr) || (m_swapChain->GetSize() != RenderAreaSize())) {
			m_swapChain = m_direct3d->CreateSwapChain(TargetWindow());
		}
	}

	DDSurface::Ptr RendererD3D::OnCreateDDSurface() {
		return DDSurface::Ptr(new DDSurfaceD3D(m_direct3d));
	}

	void RendererD3D::OnRenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props) {
		DDSurfaceD3D* ds = dynamic_cast<DDSurfaceD3D*>(dest.get());

		if(m_softTex == 0 || !m_softTex->GetSize().AtLeastInclusive(destinationArea.Dimensions()))
			m_softTex = m_direct3d->CreateTexture(destinationArea.Dimensions(), D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM);

		D3D::Texture::Lock l = m_softTex->LockRegion(RectInt(PointInt(0, 0), destinationArea.Dimensions()), false);
		try {
			Filter::FilterBuffer dst(m_softTex->GetSize(), 4, l.Buffer, l.Pitch);

			Img::FilterBufferAndLock src = GenerateFilterBuffer(source);

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

	void RendererD3D::OnPresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft, Filter::RotationAngle angle) {
		SizeFloat ppAdj{ -0.5f, -0.5f };
		auto proj = D3D::OrthogonalProjection({ { 0, 0 }, RenderAreaSize().StaticCast<float>() });
		auto middle = RenderAreaSize().StaticCast<float>() * 0.5f;
		D3DMATRIX view;

		switch (angle) {
			case Filter::RotationAngle::Rotate90:
				view = D3D::Translate(-middle.Width, -middle.Height, 0) * 
					D3D::Rotate(-Num::Pi_2) * 
					D3D::Translate(middle.Width, middle.Height, 0);
				break;
			case Filter::RotationAngle::Rotate180:
				view = D3D::Translate(-middle.Width, -middle.Height, 0) *
					D3D::Rotate(-Num::Pi) *
					D3D::Translate(middle.Width, middle.Height, 0);
				break;
			case Filter::RotationAngle::Rotate270:
				view = D3D::Translate(-middle.Width, -middle.Height, 0) * 
					D3D::Rotate(-Num::Pi - Num::Pi_2) * 
					D3D::Translate(middle.Width, middle.Height, 0);
				break;
			case Filter::RotationAngle::FlipX:
				view = D3D::Translate(-middle.Width, -middle.Height, 0) *
					D3D::Scale(-1, 1, 1) *
					D3D::Translate(middle.Width, middle.Height, 0);
				break;
			case Filter::RotationAngle::FlipY:
				view = D3D::Translate(-middle.Width, -middle.Height, 0) *
					D3D::Scale(1, -1, 1) *
					D3D::Translate(middle.Width, middle.Height, 0);
				break;
			default:
				view = D3D::Identity();
				break;
		}
		m_direct3d->SetMatrix(D3DTS_PROJECTION, &proj);
		m_direct3d->SetMatrix(D3DTS_WORLD, &view);

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
			a, b, c, d);
		m_direct3d->RenderQuad(a, b, c, d);
	}

	RendererD3D::~RendererD3D() {
		m_softTex.reset();
	}
}
