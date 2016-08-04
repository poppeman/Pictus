#include "hw3d_texture.h"

namespace Hw3D
{

	Geom::SizeInt Texture::GetSize()
	{
		return Geom::SizeInt();
	}

	Format Texture::D3DFormat()
	{
		return Format::X8R8G8B8;
	}
}