#include "w32_ddsurface.h"

namespace Win {
	void DDSurface::Create(const Geom::SizeInt& dims) {
		if (dims.Width <= 0 || dims.Height <= 0)
		{
			DO_THROW(Err::InvalidParam, "Dimensions must be non-zero.");
		}
		OnCreate(dims);
		m_dims = dims;
	}

	Geom::SizeInt DDSurface::Dimensions() {
		return m_dims;
	}
}
