#include <GL/glew.h>
#include <orz/exception.h>
#include <cstring>
#include "hw3d_opengl_staging_texture.h"
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	OpenGlStagingTexture::OpenGlStagingTexture(Geom::SizeInt dimensions, Format fmt):
		StagingTexture(dimensions, fmt)
	{
	}

	OpenGlStagingTexture::~OpenGlStagingTexture()
	{
	}
}
