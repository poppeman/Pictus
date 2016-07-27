#ifndef HW3D_OGL_TEXTURE_H
#define HW3D_OGL_TEXTURE_H

#include "types.h"

namespace Hw3D {
	class Texture {
	public:
		struct Lock {
			uint8_t* Buffer;
			size_t Pitch;
		};

		Lock LockRegion(const Geom::RectInt& region, bool readOnly);
		void UnlockRegion();

		Geom::SizeInt GetSize();

		Format D3DFormat();
	};
}

#endif

