#ifndef PICTUS_HW3D_OPENGL_STAGING_TEXTURE_H
#define PICTUS_HW3D_OPENGL_STAGING_TEXTURE_H

#include "hw3d_staging_texture.h"

namespace Hw3D
{
	class OpenGlStagingTexture:public StagingTexture
	{
	public:
		virtual void Transfer(Geom::RectInt sourceRect, Geom::PointInt destTopLeft)=0;

		OpenGlStagingTexture(Geom::SizeInt dimensions, Format fmt);
		virtual ~OpenGlStagingTexture();
	};
}

#endif
