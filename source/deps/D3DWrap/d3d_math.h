#ifndef D3D_MATH_H
#define D3D_MATH_H

#include "illa/types.h"

namespace D3D {
	// All of this is fairly horrible, but the long-term solution will probably be to migrate to D3D10/11 which
	// includes decent matrix classes out of the box (without D3DX dependencies).

	D3DMATRIX OrthogonalProjection(Geom::RectFloat zoomedArea);

	D3DMATRIX Identity();

	D3DMATRIX Rotate(float angle);
	D3DMATRIX Scale(float x, float y, float z);
	D3DMATRIX Translate(float x, float y, float z);
}

D3DMATRIX operator*(const D3DMATRIX& lhs, const D3DMATRIX& rhs);

#endif
