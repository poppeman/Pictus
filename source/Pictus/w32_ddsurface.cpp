#include "w32_ddsurface.h"

namespace Win {
	void DDSurface::Create(const Geom::SizeInt& dims) {
		COND_STRICT(dims.Width > 0 && dims.Height > 0, Err::InvalidParam, TX("Dimensions must be non-zero."));
		OnCreate(dims);
		m_dims = dims;
	}

	Geom::SizeInt DDSurface::Dimensions() {
		return m_dims;
	}
}
