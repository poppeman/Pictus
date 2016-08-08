#include <orz/types.h>
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	GLint GetGlInternalFormat(Format fmt)
	{
		return GL_RGBA;
	}

	GLenum GetGlFormat(Format)
	{
		return GL_BGRA;
	}

	GLenum GetGlDataType(Format)
	{
		return GL_UNSIGNED_BYTE;
	}

	::std::string GetGlErrorString(GLenum error)
	{
		switch (error)
		{
			case GL_NO_ERROR:
				return "No error";
			case GL_INVALID_ENUM:
				return "Invalid enum";
			case GL_INVALID_VALUE:
				return "Invalid value";
			case GL_INVALID_OPERATION:
				return "Invalid operation";
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				return "Invalid framebuffer operation";
			case GL_OUT_OF_MEMORY:
				return "Out of memory";
			case GL_STACK_UNDERFLOW:
				return "Stack underflow";
			case GL_STACK_OVERFLOW:
				return "Stack overflow";
			default:
				return "Unknown error code:" + ToAString(error);
		}
	}
}
