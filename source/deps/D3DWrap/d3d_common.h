#ifndef D3D_COMMON_H
#define D3D_COMMON_H

#define SAFE_RELEASE(p) if (p) { p->Release(); p = 0; }

#include "illa/types.h"

namespace Err {
	struct Direct3DError:public CriticalError {
		Direct3DError(const std::wstring& desc):CriticalError(TX("Direct3DError: ") + desc) {}
	};
}

namespace D3D {
	enum ErrorCode {
		ErrSucceeded,
		ErrDeviceLost,
	};

	D3DFORMAT ToD3DFORMAT(Img::Format format);

	struct Vertex2D {
		Geom::PointFloat Position;
		Geom::PointFloat TexCoord;
	};

	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment, D3D::Vertex2D& a, D3D::Vertex2D& b, D3D::Vertex2D& c, D3D::Vertex2D& d);
	void RotateQuad(Filter::RotationAngle angle, D3D::Vertex2D& a, D3D::Vertex2D& b, D3D::Vertex2D& c, D3D::Vertex2D& d);
}

#endif
