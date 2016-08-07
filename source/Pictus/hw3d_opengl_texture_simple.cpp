#include <GL/glew.h>
#include <orz/exception.h>
#include <cstring>
#include "hw3d_opengl_texture_simple.h"
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	Texture::Lock OpenGlTextureSimple::LockRegion(const Geom::RectInt &region, bool readOnly)
	{
		if(region.Width() < 0 || region.Height() < 0)
		{
			DO_THROW(Err::InvalidParam, "Attempted to lock a region with zero width or height");
		}

		Texture::Lock l;
		l.Pitch = GetSize().Width * 4;
		l.Buffer = &m_textureDataNonPBO[(region.Left() + region.Top() * GetSize().Width) * 4];

		return l;
	}

	void OpenGlTextureSimple::UnlockRegion()
	{
		GLenum err;

		glBindTexture(GL_TEXTURE_2D, m_textureName);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed binding texture: " + GetGlErrorString(err));
		}
		if (m_textureName == GL_INVALID_VALUE)
		{
			DO_THROW(Err::CriticalError, "Buggy OpenGL driver, did not get a texture object");
		}
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GetGlInternalFormat(D3DFormat()),
			GetSize().Width,
			GetSize().Height,
			0,
			GetGlFormat(D3DFormat()),
			GetGlDataType(D3DFormat()),
			&m_textureDataNonPBO[0]);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed copying data to texture: " + GetGlErrorString(err));
		}
	}

	GLuint OpenGlTextureSimple::GetTextureName()
	{
		return m_textureName;
	}

	OpenGlTextureSimple::OpenGlTextureSimple(Geom::SizeInt dimensions, Format fmt, Pool pool):
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

		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glTexImage2D failed: " + GetGlErrorString(err));
		}

		m_textureDataNonPBO.resize(dimensions.Width * dimensions.Height * 4);
	}

	OpenGlTextureSimple::~OpenGlTextureSimple()
	{
		glDeleteTextures(1, &m_textureName);
	}
}
