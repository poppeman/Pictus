#ifndef HW3D_DIRECT3D9_COMMON_H
#define HW3D_DIRECT3D9_COMMON_H

#include "orz/geom.h"
#include <d3d9.h>
#include "hw3d_types.h"
#include "orz/matrix.h"

namespace Hw3D
{
	Geom::RectInt RECTToRect(const RECT& rect);
	RECT RectToRECT(const Geom::RectInt& rect);
	POINT PointToPOINT(const Geom::PointInt& point);

	D3DMATRIX MatrixToD3DMATRIX(const Matrix& m);

	D3DFORMAT ConvertFormat(Hw3D::Format fmt);
	D3DPOOL ConvertPool(Hw3D::Pool pool);


	template<typename T> void ComRelease(T* obj)
	{
		if (obj != nullptr)
		{
			obj->Release();
		}
	}
}

#endif
