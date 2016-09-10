#include "hw3d_direct3d9_common.h"
#include "orz\exception.h"

namespace Hw3D
{
	Geom::RectInt RECTToRect(const RECT& rect)
	{
		return Geom::RectInt(Geom::PointInt(rect.left, rect.top), Geom::SizeInt(rect.right - rect.left, rect.bottom - rect.top));
	}

	RECT RectToRECT(const Geom::RectInt& rect)
	{
		RECT out_rect;
		out_rect.bottom = rect.Bottom();
		out_rect.left = rect.Left();
		out_rect.right = rect.Right();
		out_rect.top = rect.Top();
		return out_rect;
	}

	POINT PointToPOINT(const Geom::PointInt& point)
	{
		POINT out_point;
		out_point.x = point.X;
		out_point.y = point.Y;
		return out_point;
	}

	D3DMATRIX MatrixToD3DMATRIX(const Matrix& m)
	{
		D3DMATRIX mat;
		ZeroMemory(&mat, sizeof(mat));
		for (auto y = 0; y < 4; y++)
		{
			for (auto x = 0; x < 4; x++)
			{
				mat.m[y][x] = m.m[y][x];
			}
		}
		return mat;
	}

	D3DFORMAT ConvertFormat(Format fmt)
	{
		switch (fmt)
		{
		case Format::X8R8G8B8:
			return D3DFMT_X8R8G8B8;
		}

		DO_THROW(Err::InvalidParam, "Unknown format");
	}

	D3DPOOL ConvertPool(Pool pool)
	{
		switch (pool)
		{
		case Hw3D::Pool::Default:
			return D3DPOOL_DEFAULT;
		case Hw3D::Pool::SystemMemory:
			return D3DPOOL_SYSTEMMEM;
		}
		DO_THROW(Err::InvalidParam, "Unknown pool");
	}
}
