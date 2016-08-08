#ifndef PICTUS_HW3D_OPENGL_STAGING_TEXTURE_SIMPLE_H
#define PICTUS_HW3D_OPENGL_STAGING_TEXTURE_SIMPLE_H

#include "hw3d_opengl_staging_texture.h"

namespace Hw3D
{
	class OpenGlStagingTextureSimple:public OpenGlStagingTexture
	{
	public:
		Lock LockRegion(const Geom::RectInt& region, bool readOnly) override;
		void UnlockRegion() override;

		void Transfer(Geom::RectInt sourceRect, Geom::PointInt targetTopLeft) override;

		OpenGlStagingTextureSimple(Geom::SizeInt dimensions, Format fmt);
		~OpenGlStagingTextureSimple();

	private:
		std::vector<uint8_t> m_textureDataNonPBO;
	};
}

#endif
