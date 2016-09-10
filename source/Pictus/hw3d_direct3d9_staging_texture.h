#ifndef HW3D_DIRECT3D9_STAGING_TEXTURE_H
#define HW3D_DIRECT3D9_STAGING_TEXTURE_H

#include "hw3d_staging_texture.h"

#include <memory>
#include <d3d9.h>

namespace Hw3D
{
	class Direct3D9StagingTexture : public StagingTexture
	{
	public:
		Lock LockRegion(const Geom::RectInt& region, bool readOnly) override;
		void UnlockRegion() override;

		IDirect3DTexture9* D3DObject();

		Direct3D9StagingTexture(IDirect3DTexture9* tex, Geom::SizeInt dimensions, Format fmt);

	private:
		std::shared_ptr<IDirect3DTexture9> m_texture;
	};
}

#endif
