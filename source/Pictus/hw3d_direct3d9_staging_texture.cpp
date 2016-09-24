#include "hw3d_direct3d9_staging_texture.h"
#include "hw3d_direct3d9_common.h"
#include "orz\exception.h"
#include "hw3d_direct3d9_common.h"

namespace Hw3D
{
	Hw3D::StagingTexture::Lock Direct3D9StagingTexture::LockRegion(const Geom::RectInt& region, bool readOnly)
	{
		auto rect = RectToRECT(region);
		D3DLOCKED_RECT lr;
		auto ret = m_texture->LockRect(0, &lr, &rect, (readOnly ? D3DLOCK_READONLY : 0));
		if (FAILED(ret))
		{
			DO_THROW(Err::CriticalError, "Failed locking texture");
		}

		return{ static_cast<unsigned char*>(lr.pBits), static_cast<size_t>(lr.Pitch) };
	}

	void Direct3D9StagingTexture::UnlockRegion()
	{
		m_texture->UnlockRect(0);
	}

	IDirect3DTexture9* Direct3D9StagingTexture::D3DObject()
	{
		return m_texture.get();
	}

	Direct3D9StagingTexture::Direct3D9StagingTexture(IDirect3DTexture9* tex, Geom::SizeInt dimensions, Format fmt):
		StagingTexture{ dimensions, fmt },
		m_texture{ tex, ComRelease<IDirect3DTexture9> }
	{
	}
}
