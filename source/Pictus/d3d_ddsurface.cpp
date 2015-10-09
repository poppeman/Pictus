#include "d3d_ddsurface.h"

namespace Win {
	DDSurfaceD3D::DDSurfaceD3D(D3D::Device::Ptr device):m_device(device) {}

	void DDSurfaceD3D::OnCreate(const Geom::SizeInt& dims) {
		m_tex = m_device->CreateTexture(dims, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT);
	}

	D3D::Texture::Ptr DDSurfaceD3D::GetTexture() {
		return m_tex;
	}
}
