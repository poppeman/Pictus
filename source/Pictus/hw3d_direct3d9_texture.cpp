#include "hw3d_direct3d9_texture.h"
#include "hw3d_direct3d9_common.h"

namespace Hw3D
{
	IDirect3DTexture9* Direct3D9Texture::D3DObject()
	{
		return m_texture.get();
	}

	Direct3D9Texture::Direct3D9Texture(IDirect3DTexture9* tex, Geom::SizeInt dimensions, Format fmt):
		Texture{ dimensions, fmt },
		m_texture{ tex, ComRelease<IDirect3DTexture9> }
	{
	}
}
