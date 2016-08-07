#ifndef PICTUS_HW3D_OPENGL_TEXTURE_H
#define PICTUS_HW3D_OPENGL_TEXTURE_H

#include "hw3d_texture.h"

namespace Hw3D
{
	class OpenGlTexturePbo:public Texture
	{
	public:
		Lock LockRegion(const Geom::RectInt& region, bool readOnly);
		void UnlockRegion();

		GLuint GetTextureName();

		OpenGlTexturePbo(Geom::SizeInt dimensions, Format fmt, Pool pool);
		~OpenGlTexturePbo();

	private:
		GLuint m_textureName;
		GLuint m_bufferObject;
	};
}

#endif
