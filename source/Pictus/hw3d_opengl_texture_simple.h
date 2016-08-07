#ifndef PICTUS_HW3D_OPENGL_TEXTURE_SIMPLE_H
#define PICTUS_HW3D_OPENGL_TEXTURE_SIMPLE_H

#include "hw3d_texture.h"

namespace Hw3D
{
	class OpenGlTextureSimple:public Texture
	{
	public:
		Lock LockRegion(const Geom::RectInt& region, bool readOnly);
		void UnlockRegion();

		GLuint GetTextureName();

		OpenGlTextureSimple(Geom::SizeInt dimensions, Format fmt, Pool pool);
		~OpenGlTextureSimple();

	private:
		GLuint m_textureName;
		std::vector<uint8_t> m_textureDataNonPBO;
	};
}

#endif
