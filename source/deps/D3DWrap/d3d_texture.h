#ifndef D3D_TEXTURE_H
#define D3D_TEXTURE_H

#include "illa/types.h"

namespace D3D {
	class Texture final {
	public:
		Texture(LPDIRECT3DTEXTURE9 tex);

		struct Lock {
			uint8_t* Buffer;
			size_t Pitch;
		};

		Lock LockRegion(const Geom::RectInt& region, bool readOnly);
		void UnlockRegion();

		Geom::SizeInt GetSize();

		D3DFORMAT D3DFormat();
		LPDIRECT3DTEXTURE9 D3DObject();

		typedef std::shared_ptr<Texture> Ptr;

	private:
		std::shared_ptr<IDirect3DTexture9> m_texture;
	};
}

#endif
