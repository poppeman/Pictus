#include <GL/glew.h>
#include <orz/exception.h>
#include <cstring>
#include "hw3d_opengl_texture.h"
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	GLuint OpenGlTexture::GetTextureName()
	{
		return m_textureName;
	}

	OpenGlTexture::OpenGlTexture(Geom::SizeInt dimensions, Format fmt, Pool pool):
		Texture(dimensions, fmt),
		m_textureName(0)
	{
		GLenum err;

		// Create texture
		glGenTextures(1, &m_textureName);
		if(m_textureName == GL_INVALID_VALUE)
		{
			m_textureName = 0;
			DO_THROW(Err::CriticalError, "Failed allocating texture");
		}
		glBindTexture(GL_TEXTURE_2D, m_textureName);
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glBindTexture failed: " + GetGlErrorString(err));
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );

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

		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glTexImage2D failed: " + GetGlErrorString(err));
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	OpenGlTexture::~OpenGlTexture()
	{
		glDeleteTextures(1, &m_textureName);
		m_textureName = 0;
	}
}
