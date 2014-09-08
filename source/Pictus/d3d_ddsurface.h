#ifndef D3D_DDSURFACE_H
#define D3D_DDSURFACE_H

#include "w32_ddsurface.h"
#include "D3DWrap/d3d_device.h"
#include "D3DWrap/d3d_texture.h"

namespace Win {
	class DDSurfaceD3D:public DDSurface {
	public:
		DDSurfaceD3D(D3D::Device::Ptr device);
		D3D::Texture::Ptr GetTexture();

	private:
		void OnCreate(const Geom::SizeInt& dims);

		D3D::Device::Ptr m_device;
		D3D::Texture::Ptr m_tex;
	};
}

#endif
