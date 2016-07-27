#include "w32_renderer.h"
#include "Hw3D/matrix.h"
#include "illa/swsurface.h"
#include "d3d_ddsurface.h"
#include "illa/render.h"

#include <algorithm>

namespace Win {
	using namespace Geom;

	Geom::SizeInt Renderer::RenderAreaSize() {
		//RECT cr;
		//GetClientRect(TargetWindow(), &cr);
		//return{ cr.right - cr.left, cr.bottom - cr.top };
		int w, h;
		TargetWindow()->GetClientSize(&w, &h);
		return {w, h};
	}

	Geom::SizeInt Renderer::TransformedRenderAreaSize() {
		return Transform(RenderAreaSize());
	}


	Geom::SizeInt Renderer::Transform(Geom::SizeInt sz) {
		switch (Angle) {
			case Filter::RotationAngle::Rotate90:
			case Filter::RotationAngle::Rotate90FlipY:
			case Filter::RotationAngle::Rotate270:
			case Filter::RotationAngle::Rotate270FlipY:
				return{ sz.Height, sz.Width };
		}
		return sz;
	}

	// Transform pan coordinates from screen-space to image-space. Since flipping and rotation is done
	// during presentation (and thus hardware-based), we need convert the panning coordinates.
	Geom::PointInt Renderer::TransformPan(Geom::PointInt pan, Geom::SizeInt imageSize) {
		auto ras = RenderAreaSize();
		switch (Angle) {
		case Filter::RotationAngle::RotateDefault:
			return pan;
		case Filter::RotationAngle::FlipX:
			return{ std::max(0, (imageSize.Width - ras.Width) - pan.X), pan.Y };

		case Filter::RotationAngle::FlipY:
			return{ pan.X, std::max(0, (imageSize.Height - ras.Height) - pan.Y) };

		case Filter::RotationAngle::Rotate90:
			return{ pan.Y, std::max(0, (imageSize.Height - ras.Width) - pan.X) };

		case Filter::RotationAngle::Rotate90FlipY:
			return{ std::max(0, (imageSize.Width - ras.Height) - pan.Y), std::max(0, (imageSize.Height - ras.Width) - pan.X) };

		case Filter::RotationAngle::Rotate180:
			return{ std::max(0, (imageSize.Width - ras.Width) - pan.X), std::max(0, (imageSize.Height - ras.Height) - pan.Y) };

		case Filter::RotationAngle::Rotate270:
			return{ std::max(0, (imageSize.Width - ras.Height) - pan.Y), pan.X };

		case Filter::RotationAngle::Rotate270FlipY:
			return{ pan.Y, pan.X };
		}
		DO_THROW(Err::InvalidCall, "Unsupported angle");
	}

	bool Renderer::TargetWindow( wxWindow* hwnd ) {
		m_hwnd = hwnd;

		if (m_direct3d == nullptr) {
			m_direct3d = std::make_shared<Hw3D::Device>();
		}

		if (m_direct3d->Initialize(TargetWindow()) == false) {
			m_direct3d.reset();
			return false;
		}
		return true;

	}

	Renderer::RenderStatus Renderer::BeginRender(Img::Color backgroundColor) {
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

		auto proj = Hw3D::OrthographicProjection({ { 0, 0 }, RenderAreaSize().StaticCast<float>() });
		m_direct3d->SetMatrix(Hw3D::TransformState::Projection, proj);

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

	wxWindow* Renderer::TargetWindow() { return m_hwnd; }

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
			m_softTex = m_direct3d->CreateTexture(destinationArea.Dimensions(), Hw3D::Format::X8R8G8B8, Hw3D::Pool::SystemMemory);
		}

		Hw3D::Texture::Lock l = m_softTex->LockRegion(RectInt(PointInt(0, 0), destinationArea.Dimensions()), false);
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

		Hw3D::Vertex2D a, b, c, d;
		auto uvTL = sourceTopLeft.StaticCast<float>() / source->Dimensions().StaticCast<float>();
		auto uvBR = (sourceTopLeft + destRect.Dimensions()).StaticCast<float>() / source->Dimensions().StaticCast<float>();
		Hw3D::GenerateQuad(
			destRect.StaticCast<float>(),
			{ uvTL, uvBR },
			ppAdj,
			RenderAreaSize(),
			Angle,
			a, b, c, d);
		m_direct3d->RenderQuad(a, b, c, d);
	}

}
