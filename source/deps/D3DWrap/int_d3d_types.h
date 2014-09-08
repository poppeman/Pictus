#ifndef INT_D3D_TYPES_H
#define INT_D3D_TYPES_H

#include "illa/types.h"

namespace D3D {
	Geom::RectInt RECTToRect(const RECT& rect);
	RECT RectToRECT(const Geom::RectInt& rect);
	POINT PointToPOINT(const Geom::PointInt& point);
}

#endif
