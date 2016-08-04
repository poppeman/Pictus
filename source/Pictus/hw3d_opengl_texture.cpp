#include <GL/gl.h>
#include <orz/exception.h>
#include "hw3d_opengl_texture.h"
#include "hw3d_opengl_common.h"

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
		glBindTexture(GL_TEXTURE_2D, m_textureName);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GetGlInternalFormat(fmt),
			dimensions.Width,
			dimensions.Height,
			0,
			GetGlFormat(fmt),
			GetGlDataType(fmt),
			nullptr
		);

		GLenum err;

		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glTexImage2D failed: " + GetGlErrorString(err));
		}
	}

	OpenGlTexture::~OpenGlTexture()
	{
		glDeleteTextures(1, &m_textureName);
	}
}
