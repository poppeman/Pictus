#include "hw3d_opengl_texture.h"

namespace Hw3D
{
	Texture::Lock OpenGlTexture::LockRegion(const Geom::RectInt &region, bool readOnly)
	{
		return Texture::Lock();
	}

	void OpenGlTexture::UnlockRegion()
	{

	}
}
