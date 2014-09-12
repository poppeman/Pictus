#ifndef RENDERTARGETD3D_H
#define RENDERTARGETD3D_H

#include "D3DWrap/d3d_device.h"
#include "D3DWrap/d3d_swapchain.h"
#include "w32_rendertarget.h"

namespace Img {
	class SurfaceSoftware;
}

namespace Win {
	class RendererD3D:public Renderer {
	public:
		Img::Surface::Ptr CreateSurface();

		~RendererD3D();

	private:
		Geom::SizeInt RenderAreaSize();

		bool OnTargetWindowChanged();

		RenderStatus OnBeginRender(Img::Color backgroundColor) override;
		void OnEndRender();

		void CreateTextures();

		DDSurface::Ptr OnCreateDDSurface();
		void OnRenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props);
		void OnPresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft);

		enum {
			MaximumTileEdgeLength = 512,
			VbFmt = D3D::VFPositionXYZ | D3D::VFTex01,
		};

		struct Vertex {
			float x, y, z;
			float u, v;
		};

		D3D::Texture::Ptr m_softTex;
		D3D::Device::Ptr m_direct3d;
		D3D::SwapChain::Ptr m_swapChain;
	};
}

#endif
