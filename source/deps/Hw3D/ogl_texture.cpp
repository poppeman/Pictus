#include <orz/geom.h>
#include "ogl_texture.h"

namespace Hw3D {
	Texture::Lock Texture::LockRegion(const Geom::RectInt &region, bool readOnly)
	{
		return Texture::Lock();
	}

	void Texture::UnlockRegion()
	{

	}

	Geom::SizeInt Texture::GetSize()
	{
		return Geom::Size<int>();
	}

}
