#ifndef HW3D_DIRECT3D9_TEXTURE_H
#define HW3D_DIRECT3D9_TEXTURE_H

#include "hw3d_texture.h"
#include <memory>
#include <d3d9.h>

namespace Hw3D
{
	class Direct3D9Texture : public Texture
	{
	public:
		IDirect3DTexture9* D3DObject();

		Direct3D9Texture(IDirect3DTexture9* tex, Geom::SizeInt dimensions, Format fmt);

	private:
		std::shared_ptr<IDirect3DTexture9> m_texture;
	};
}

#endif
