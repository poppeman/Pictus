#ifndef PICTUS_HW3D_STAGING_TEXTURE_H
#define PICTUS_HW3D_STAGING_TEXTURE_H

#include <cstdint>
#include <cstddef>
#include "hw3d_types.h"
#include "orz/geom.h"

namespace Hw3D
{
	class StagingTexture
	{
	public:
		struct Lock {
			uint8_t* Buffer;
			size_t Pitch;
		};

		virtual Lock LockRegion(const Geom::RectInt& region, bool readOnly)=0;
		virtual void UnlockRegion()=0;

		Geom::SizeInt GetSize();

		Format D3DFormat();

		StagingTexture(Geom::SizeInt dimensions, Format fmt);

	private:
		Geom::SizeInt m_size;
		Format m_format;
	};
}

#endif
