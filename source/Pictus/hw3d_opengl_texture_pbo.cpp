#include <GL/glew.h>
#include <GL/gl.h>
#include <orz/exception.h>
#include <cstring>
#include "hw3d_opengl_texture_pbo.h"
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	Texture::Lock OpenGlTexturePbo::LockRegion(const Geom::RectInt &region, bool readOnly)
	{
		if (region.Width() < 0 || region.Height() < 0)
		{
			DO_THROW(Err::InvalidParam, "Attempted to lock a region with zero width or height");
		}
		m_lockedArea = region;

		Texture::Lock l;
		l.Pitch = GetSize().Width * 4;
		auto bytesToLock = (region.Width() + (region.Height() - 1) * GetSize().Width) * 4;
		auto offset = (region.Left() + region.Top() * GetSize().Width) * 4;
		bytesToLock = GetSize().Width * GetSize().Height * 4;

		GLenum err;
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_bufferObject);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed binding buffer: " + GetGlErrorString(err));
		}

		GLbitfield flags = 0;
		if (!readOnly)
		{
			flags |= GL_MAP_WRITE_BIT;
		}

		auto ptr = glMapBufferRange(
			GL_PIXEL_UNPACK_BUFFER,
			0,
			bytesToLock,
			flags);

		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed mapping texture buffer: " + GetGlErrorString(err));
		}

		if (ptr == nullptr)
		{
			DO_THROW(Err::CriticalError, "Failed mapping buffer object");
		}

		l.Buffer = reinterpret_cast<uint8_t *>(ptr) + offset;

		return l;
	}

	void OpenGlTexturePbo::UnlockRegion()
	{
		GLenum err;

		glBindTexture(GL_TEXTURE_2D, m_textureName);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed binding texture: " + GetGlErrorString(err));
		}

		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed unmapping buffer object: " + GetGlErrorString(err));
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
			nullptr
		);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed copying data to texture: " + GetGlErrorString(err));
		}

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glBindBuffer failed: " + GetGlErrorString(err));
		}
	}

	OpenGlTexturePbo::OpenGlTexturePbo(Geom::SizeInt dimensions, Format fmt, Pool pool):
		OpenGlTexture(dimensions, fmt, pool),
		m_bufferObject(0)
	{
		GLenum err;

		// Create texture
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glTexImage2D failed: " + GetGlErrorString(err));
		}

		// Create buffer object that will be used for transfers during Lock/Unlock
		glGenBuffers(1, &m_bufferObject);
		if(m_bufferObject == GL_INVALID_VALUE)
		{
			DO_THROW(Err::CriticalError, "Failed allocating PBO");
		}
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_bufferObject);
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed binding buffer: " + GetGlErrorString(err));
		}
		glBufferData(GL_PIXEL_UNPACK_BUFFER, dimensions.Width * dimensions.Height * 4, nullptr, GL_STREAM_DRAW);
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed allocating buffer data: " + GetGlErrorString(err));
		}

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "glBindBuffer failed: " + GetGlErrorString(err));
		}
	}

	OpenGlTexturePbo::~OpenGlTexturePbo()
	{
		glDeleteBuffers(1, &m_bufferObject);
	}
}
