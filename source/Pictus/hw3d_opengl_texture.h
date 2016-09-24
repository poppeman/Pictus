#ifndef PICTUS_HW3D_OPENGL_TEXTURE_H
#define PICTUS_HW3D_OPENGL_TEXTURE_H

#include "hw3d_texture.h"

namespace Hw3D
{
	class OpenGlTexture:public Texture
	{
	public:
		GLuint GetTextureName();

		OpenGlTexture(Geom::SizeInt dimensions, Format fmt, Pool pool);
		virtual ~OpenGlTexture();

	protected:
		GLuint m_textureName;
	};
}

#endif
