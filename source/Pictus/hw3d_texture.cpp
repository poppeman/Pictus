#include "hw3d_texture.h"

namespace Hw3D
{

	Geom::SizeInt Texture::GetSize()
	{
		return m_size;
	}

	Format Texture::D3DFormat()
	{
		return m_format;
	}

	Texture::Texture(Geom::SizeInt dimensions, Format fmt):
		m_size(dimensions),
		m_format(fmt)
	{
	}
}