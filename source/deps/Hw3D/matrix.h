#ifndef D3DWRAP_D3D_MATH_H
#define D3DWRAP_D3D_MATH_H

#include "illa/types.h"

namespace Hw3D {
	struct Matrix {
		float m[4][4];
	};

	Matrix OrthographicProjection(Geom::RectFloat zoomedArea);

	Matrix Identity();

	Matrix Rotate(float angle);
	Matrix Scale(float x, float y, float z);
	Matrix Translate(float x, float y, float z);

	struct Vertex2D {
		Geom::PointFloat Position;
		Geom::PointFloat TexCoord;
	};

	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment, Geom::SizeInt targetSize, Filter::RotationAngle angle, Vertex2D& a, Vertex2D& b, Vertex2D& c, Vertex2D& d);
}

Hw3D::Matrix operator*(const Hw3D::Matrix& lhs, const Hw3D::Matrix& rhs);

#endif
