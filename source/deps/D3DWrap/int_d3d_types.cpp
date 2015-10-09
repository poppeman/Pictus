#include "int_d3d_types.h"

namespace D3D {
	Geom::RectInt RECTToRect(const RECT& rect) {
		return Geom::RectInt(Geom::PointInt(rect.left, rect.top), Geom::SizeInt(rect.right-rect.left, rect.bottom-rect.top));
	}

	RECT RectToRECT(const Geom::RectInt& rect) {
		RECT out_rect;
		out_rect.bottom	= rect.Bottom();
		out_rect.left	= rect.Left();
		out_rect.right	= rect.Right();
		out_rect.top	= rect.Top();
		return out_rect;
	}

	POINT PointToPOINT( const Geom::PointInt& point ) {
		POINT out_point;
		out_point.x = point.X;
		out_point.y = point.Y;
		return out_point;
	}
}