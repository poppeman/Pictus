#ifndef D3D_DDSURFACE_H
#define D3D_DDSURFACE_H

#include "w32_ddsurface.h"
#include "hw3d_device.h"
#include "hw3d_texture.h"

namespace Win {
	class DDSurfaceD3D:public DDSurface {
	public:
		DDSurfaceD3D(std::shared_ptr<Hw3D::Device> device);
		std::shared_ptr<Hw3D::Texture> GetTexture();

	private:
		void OnCreate(const Geom::SizeInt& dims);

		std::shared_ptr<Hw3D::Device> m_device;
		std::shared_ptr<Hw3D::Texture> m_tex;
	};
}

#endif
