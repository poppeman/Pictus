#include "hw3d_staging_texture.h"

namespace Hw3D
{
	Geom::SizeInt StagingTexture::GetSize()
	{
		return m_size;
	}

	Format StagingTexture::D3DFormat()
	{
		return m_format;
	}

	StagingTexture::StagingTexture(Geom::SizeInt dimensions, Format fmt):
		m_size(dimensions),
		m_format(fmt)
	{
	}
}
