#ifndef D3DWRAP_INT_D3D_TYPES_H
#define D3DWRAP_INT_D3D_TYPES_H

#include "illa/types.h"

#include <windows.h>

namespace D3D {
	Geom::RectInt RECTToRect(const RECT& rect);
	RECT RectToRECT(const Geom::RectInt& rect);
	POINT PointToPOINT(const Geom::PointInt& point);
}

#endif
