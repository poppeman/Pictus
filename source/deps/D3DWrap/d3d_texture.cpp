#include "StdAfx.h"

#include "d3d_texture.h"
#include "d3d_common.h"
#include "int_d3d_types.h"

namespace D3D {
	Texture::Lock Texture::LockRegion(const Geom::RectInt& region, bool readOnly) {
		RECT rect = RectToRECT(region);
		D3DLOCKED_RECT lr;
		auto ret = m_texture->LockRect(0, &lr, &rect, (readOnly ? D3DLOCK_READONLY : 0));
		if (FAILED(ret)) {
			DO_THROW(Err::Direct3DError, L"Failed locking texture");
		}

		return{ static_cast<unsigned char*>(lr.pBits), lr.Pitch };
	}

	void Texture::UnlockRegion() {
		m_texture->UnlockRect(0);
	}

	Geom::SizeInt Texture::GetSize() {
		if (m_texture == nullptr) {
			DO_THROW(Err::CriticalError, TX("RenderTarget not created."));
		}
		LPDIRECT3DSURFACE9 surface;
		m_texture->GetSurfaceLevel(0, &surface);

		D3DSURFACE_DESC desc;
		surface->GetDesc(&desc);
		surface->Release();

		return Geom::SizeInt(desc.Width, desc.Height);
	}

	Texture::Texture(LPDIRECT3DTEXTURE9 tex)
		:m_texture{ tex, ComRelease < IDirect3DTexture9 > }
	{}

	LPDIRECT3DTEXTURE9 Texture::D3DObject() {
		if (m_texture == nullptr) {
			DO_THROW(Err::CriticalError, TX("Object not yet created."));
		}
		return m_texture.get();
	}

	D3DFORMAT Texture::D3DFormat() {
		D3DSURFACE_DESC desc;
		m_texture->GetLevelDesc(0, &desc);
		return desc.Format;
	}
}
