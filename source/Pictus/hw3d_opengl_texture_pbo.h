#ifndef PICTUS_HW3D_OPENGL_TEXTURE_PBO_H
#define PICTUS_HW3D_OPENGL_TEXTURE_PBO_H

#include "hw3d_opengl_texture.h"

namespace Hw3D
{
	class OpenGlTexturePbo:public OpenGlTexture
	{
	public:
		Lock LockRegion(const Geom::RectInt& region, bool readOnly);
		void UnlockRegion();

		OpenGlTexturePbo(Geom::SizeInt dimensions, Format fmt, Pool pool);
		~OpenGlTexturePbo();

	private:
		GLuint m_bufferObject;
		Geom::RectInt m_lockedArea;
	};
}

#endif
