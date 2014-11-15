#include "StdAfx.h"

#include "d3d_math.h"

namespace D3D {
	using Geom::RectFloat;

	D3DMATRIX OrthographicProjection(RectFloat zoomedArea) {
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

	D3DMATRIX Rotate(const float angle) {
		auto cosT = cos(angle);
		auto sinT = sin(angle);

		D3DMATRIX rot;
		ZeroMemory(&rot, sizeof(rot));
		rot._11 = cosT;
		rot._12 = -sinT;
		rot._21 = sinT;
		rot._22 = cosT;
		rot._33 = 1;
		rot._44 = 1;
		return rot;
	}

	D3DMATRIX Scale(const float x, const float y, const float z) {
		D3DMATRIX scale;
		ZeroMemory(&scale, sizeof(scale));
		scale._11 = x;
		scale._22 = y;
		scale._33 = z;
		scale._44 = 1;
		return scale;
	}

	D3DMATRIX Translate(float x, float y, float z) {
		D3DMATRIX out;
		ZeroMemory(&out, sizeof(out));
		out._11 = 1;
		out._41 = x;
		out._22 = 1;
		out._42 = y;
		out._33 = 1;
		out._43 = z;
		out._44 = 1;
		return out;
	}

	D3DMATRIX Identity() {
		D3DMATRIX out;
		ZeroMemory(&out, sizeof(out));
		out._11 = 1.0f;
		out._22 = 1.0f;
		out._33 = 1.0f;
		out._44 = 1.0f;
		return out;
	}
}

D3DMATRIX operator*(const D3DMATRIX& lhs, const D3DMATRIX& rhs) {
	D3DMATRIX out;
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
