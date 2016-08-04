#ifndef PICTUS_HW3D_OPENGL_COMMON_H
#define PICTUS_HW3D_OPENGL_COMMON_H

#include <GL/gl.h>
#include <string>
#include "hw3d_types.h"

namespace Hw3D
{
	GLint GetGlInternalFormat(Format);
	GLenum GetGlFormat(Format);
	GLenum GetGlDataType(Format);

	::std::string GetGlErrorString(GLenum);
}

#endif

