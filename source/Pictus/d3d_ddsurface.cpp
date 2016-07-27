#include "d3d_ddsurface.h"

namespace Win {
	DDSurfaceD3D::DDSurfaceD3D(std::shared_ptr<Hw3D::Device> device):m_device(device) {}

	void DDSurfaceD3D::OnCreate(const Geom::SizeInt& dims) {
		m_tex = m_device->CreateTexture(dims, Hw3D::Format::X8R8G8B8, Hw3D::Pool::Default);
	}

	std::shared_ptr<Hw3D::Texture> DDSurfaceD3D::GetTexture() {
		return m_tex;
	}
}
