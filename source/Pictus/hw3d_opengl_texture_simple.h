#ifndef PICTUS_HW3D_OPENGL_TEXTURE_SIMPLE_H
#define PICTUS_HW3D_OPENGL_TEXTURE_SIMPLE_H

#include "hw3d_opengl_texture.h"

namespace Hw3D
{
	class OpenGlTextureSimple:public OpenGlTexture
	{
	public:
		Lock LockRegion(const Geom::RectInt& region, bool readOnly);
		void UnlockRegion();

		OpenGlTextureSimple(Geom::SizeInt dimensions, Format fmt, Pool pool);
		~OpenGlTextureSimple();

	private:
		std::vector<uint8_t> m_textureDataNonPBO;
	};
}

#endif
