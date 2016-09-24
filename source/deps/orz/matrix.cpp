#include <cstring>
#include "matrix.h"

namespace Hw3D
{
	using Geom::RectFloat;

	Matrix OrthographicProjection(RectFloat zoomedArea)
	{
		Matrix proj;
		memset(&proj, 0, sizeof(proj));
		proj.m[0][0] = 2.0f / (zoomedArea.Right() - zoomedArea.Left());
		proj.m[1][1] = 2.0f / (zoomedArea.Top() - zoomedArea.Bottom());
		proj.m[2][2] = 1 / (1 - 0);
		proj.m[3][0] = (zoomedArea.Left() + zoomedArea.Right()) / (zoomedArea.Left() - zoomedArea.Right());
		proj.m[3][1] = (zoomedArea.Top() + zoomedArea.Bottom()) / (zoomedArea.Bottom() - zoomedArea.Top());
		proj.m[3][2] = 0 / (0 - 1);
		proj.m[3][3] = 1;
		return proj;
	}

	Matrix Rotate(const float angle)
	{
		auto cosT = cos(angle);
		auto sinT = sin(angle);

		Matrix rot;
		memset(&rot, 0, sizeof(rot));
		rot.m[0][0] = cosT;
		rot.m[0][1] = -sinT;
		rot.m[1][0] = sinT;
		rot.m[1][1] = cosT;
		rot.m[2][2] = 1;
		rot.m[3][3] = 1;
		return rot;
	}

	Matrix Scale(const float x, const float y, const float z)
	{
		Matrix scale;
		memset(&scale, 0, sizeof(scale));
		scale.m[0][0] = x;
		scale.m[1][1] = y;
		scale.m[2][2] = z;
		scale.m[3][3] = 1;
		return scale;
	}

	Matrix Translate(float x, float y, float z)
	{
		Matrix out;
		memset(&out, 0, sizeof(out));
		out.m[0][0] = 1;
		out.m[3][0] = x;
		out.m[1][1] = 1;
		out.m[3][1] = y;
		out.m[2][2] = 1;
		out.m[3][2] = z;
		out.m[3][3] = 1;
		return out;
	}

	Matrix Identity()
	{
		Matrix out;
		memset(&out, 0, sizeof(out));
		out.m[0][0] = 1.0f;
		out.m[1][1] = 1.0f;
		out.m[2][2] = 1.0f;
		out.m[3][3] = 1.0f;
		return out;
	}
}

Hw3D::Matrix operator*(const Hw3D::Matrix& lhs, const Hw3D::Matrix& rhs) {
	Hw3D::Matrix out;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			out.m[y][x] =
				lhs.m[y][0] * rhs.m[0][x] +
				lhs.m[y][1] * rhs.m[1][x] +
				lhs.m[y][2] * rhs.m[2][x] +
				lhs.m[y][3] * rhs.m[3][x];
		}
	}
	return out;
}
