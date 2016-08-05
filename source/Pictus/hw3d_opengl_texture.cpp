#include <GL/glew.h>
#include <GL/gl.h>
#include <orz/exception.h>
#include "hw3d_opengl_texture.h"
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	Texture::Lock OpenGlTexture::LockRegion(const Geom::RectInt &region, bool readOnly)
	{
		if(region.Width() < 0 || region.Height() < 0)
		{
			DO_THROW(Err::InvalidParam, "Attempted to lock a region with zero width or height");
		}
		GLenum err;
		glBindTexture(GL_TEXTURE_2D, m_textureName);
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed binding texture:" + GetGlErrorString(err));
		}
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_bufferObject);
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed binding buffer: " + GetGlErrorString(err));
		}

		auto offset = (region.Left() + region.Top() * GetSize().Width) * 4;
		auto length = (region.Width() + (region.Height() - 1) * GetSize().Width) * 4;
		GLbitfield flags = GL_MAP_READ_BIT;
		if(!readOnly)
		{
			flags |= GL_MAP_WRITE_BIT;
		}

		auto ptr = glMapBufferRange(
			GL_PIXEL_UNPACK_BUFFER,
			offset,
			length,
			flags);
		//auto ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed mapping texture buffer: " + GetGlErrorString(err));
		}

		if(ptr == nullptr)
		{
			DO_THROW(Err::CriticalError, "Failed mapping texture buffer");
		}

		Texture::Lock l;
		l.Buffer = reinterpret_cast<uint8_t*>(ptr);
		l.Pitch = GetSize().Width * 4;

		return l;
	}

	void OpenGlTexture::UnlockRegion()
	{
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}

	OpenGlTexture::OpenGlTexture(Geom::SizeInt dimensions, Format fmt, Pool pool):
		Texture(dimensions, fmt),
		m_textureName(0),
		m_bufferObject(0)
	{
		glGenTextures(1, &m_textureName);
		if(m_textureName == GL_INVALID_VALUE)
		{
			m_textureName = 0;
			DO_THROW(Err::CriticalError, "Failed allocating texture");
		}
		glBindTexture(GL_TEXTURE_2D, m_textureName);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
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
		//glBufferData(GL_PIXEL_UNPACK_BUFFER, num_bytes, NULL, GL_STREAM_DRAW);
	}

	OpenGlTexture::~OpenGlTexture()
	{
		glDeleteBuffers(1, &m_bufferObject);
		glDeleteTextures(1, &m_textureName);
	}
}
