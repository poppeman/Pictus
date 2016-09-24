#ifndef PICTUS_HW3D_OPENGL_STAGING_TEXTURE_PBO_H
#define PICTUS_HW3D_OPENGL_STAGING_TEXTURE_PBO_H

#include "hw3d_opengl_staging_texture.h"

namespace Hw3D
{
	class OpenGlStagingTexturePbo:public OpenGlStagingTexture
	{
	public:
		StagingTexture::Lock LockRegion(const Geom::RectInt& region, bool readOnly) override;
		void UnlockRegion() override;

		void Transfer(Geom::RectInt sourceRect, Geom::PointInt destTopLeft) override;

		OpenGlStagingTexturePbo(Geom::SizeInt dimensions, Format fmt);
		~OpenGlStagingTexturePbo();

	private:
		GLuint m_bufferObject;
		Geom::RectInt m_lockedArea;
	};
}

#endif
