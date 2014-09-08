#include "StdAfx.h"

#include "d3d_math.h"

namespace D3D {
	using Geom::RectFloat;

	D3DMATRIX OrthogonalProjection( const RectFloat &zoomedArea ) {
		D3DMATRIX proj;
		ZeroMemory(&proj, sizeof(proj));
		proj._11 = 2.0f / (zoomedArea.Right() - zoomedArea.Left());
		proj._22 = 2.0f / (zoomedArea.Top() - zoomedArea.Bottom());
		proj._33 = 1 / (1 - 0);
		proj._41 = (zoomedArea.Left() + zoomedArea.Right()) / (zoomedArea.Left() - zoomedArea.Right());
		proj._42 = (zoomedArea.Top() + zoomedArea.Bottom()) / (zoomedArea.Bottom() - zoomedArea.Top());
		proj._43 = 0 / (0 - 1);
		proj._44 = 1;
		return proj;
	}
}