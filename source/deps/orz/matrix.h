#ifndef D3DWRAP_D3D_MATH_H
#define D3DWRAP_D3D_MATH_H

#include "geom.h"

namespace Hw3D {
	struct Matrix {
		float m[4][4] = { { 0.0f } };
	};

	Matrix OrthographicProjection(Geom::RectFloat zoomedArea);

	Matrix Identity();

	Matrix Rotate(float angle);
	Matrix Scale(float x, float y, float z);
	Matrix Translate(float x, float y, float z);
}

Hw3D::Matrix operator*(const Hw3D::Matrix& lhs, const Hw3D::Matrix& rhs);

#endif
