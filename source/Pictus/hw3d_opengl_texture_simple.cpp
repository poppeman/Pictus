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
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			GetSize().Width,
			GetSize().Height,
			GetGlFormat(D3DFormat()),
			GetGlDataType(D3DFormat()),
			&m_textureDataNonPBO[0]);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed copying data to texture: " + GetGlErrorString(err));
		}
	}

	OpenGlTextureSimple::OpenGlTextureSimple(Geom::SizeInt dimensions, Format fmt, Pool pool):
		OpenGlTexture(dimensions, fmt, pool)
	{
		m_textureDataNonPBO.resize(dimensions.Width * dimensions.Height * 4);
	}

	OpenGlTextureSimple::~OpenGlTextureSimple()
	{
	}
}
