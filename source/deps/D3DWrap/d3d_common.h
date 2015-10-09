#ifndef D3DWRAP_D3D_COMMON_H
#define D3DWRAP_D3D_COMMON_H

#include "illa/types.h"
#include <d3d9.h>

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

	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment, Geom::SizeInt targetSize, Filter::RotationAngle angle, D3D::Vertex2D& a, D3D::Vertex2D& b, D3D::Vertex2D& c, D3D::Vertex2D& d);

	template<typename T> void ComRelease(T* obj) {
		if (obj != nullptr) {
			obj->Release();
		}
	}
}

#endif
