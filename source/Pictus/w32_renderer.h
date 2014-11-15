#ifndef W32_RENDERER_H
#define W32_RENDERER_H

#include "illa/surface.h"
#include "w32_ddsurface.h"
#include "D3DWrap/d3d_device.h"
#include "D3DWrap/d3d_swapchain.h"

namespace Win {
	class Renderer final {
	public:
		Filter::RotationAngle Angle;

		Img::Surface::Ptr CreateSurface();
		DDSurface::Ptr CreateDDSurface();

		Geom::SizeInt TransformedRenderAreaSize();
		//Geom::PointInt Transform(Geom::PointInt sz, Geom::SizeInt areaSize);
		Geom::SizeInt Transform(Geom::SizeInt sz);

		bool TargetWindow(HWND hwnd);

		enum class RenderStatus {
			CurrentViewLost,
			OK,
		};

		RenderStatus BeginRender(Img::Color backgroundColor);
		void RenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props);

		void PresentFromDDSurface(Geom::RectInt destRect, DDSurface::Ptr source, Geom::PointInt sourceTopLeft);

		void EndRender();

		Renderer();
		~Renderer();

		typedef std::shared_ptr<Renderer> Ptr;

	protected:
		HWND TargetWindow();

	private:
		HWND m_hwnd;

		enum {
			MaximumTileEdgeLength = 512,
			VbFmt = D3D::VFPositionXYZ | D3D::VFTex01,
		};

		struct Vertex {
			float x, y, z;
			float u, v;
		};

		void CreateTextures();
		Geom::SizeInt RenderAreaSize();

		D3D::Texture::Ptr m_softTex;
		D3D::Device::Ptr m_direct3d;
		D3D::SwapChain::Ptr m_swapChain;
	};
}

#endif
