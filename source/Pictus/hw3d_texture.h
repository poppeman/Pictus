#ifndef PICTUS_HW3D_TEXTURE_H
#define PICTUS_HW3D_TEXTURE_H

#include <cstdint>
#include <cstddef>
#include "hw3d_types.h"
#include "orz/geom.h"

namespace Hw3D {
	class Texture {
	public:
		struct Lock {
			uint8_t* Buffer;
			size_t Pitch;
		};

		virtual Lock LockRegion(const Geom::RectInt& region, bool readOnly)=0;
		virtual void UnlockRegion()=0;

		virtual Geom::SizeInt GetSize()=0;

		virtual Format D3DFormat()=0;
	};
}

#endif
