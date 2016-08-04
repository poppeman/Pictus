#include <GL/gl.h>
#include <orz/exception.h>
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

	OpenGlTexture::OpenGlTexture(Geom::SizeInt dimensions, Format fmt, Pool pool):
		Texture(dimensions, fmt),
		m_textureName(0)
	{
		glGenTextures(1, &m_textureName);
		if(m_textureName == GL_INVALID_VALUE)
		{
			m_textureName = 0;
			DO_THROW(Err::CriticalError, "Failed allocating texture");
		}
	}

	OpenGlTexture::~OpenGlTexture()
	{
		glDeleteTextures(1, &m_textureName);
	}
}
