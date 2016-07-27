#ifndef D3DWRAP_D3D_COMMON_H
#define D3DWRAP_D3D_COMMON_H

#include "illa/types.h"
#include <d3d9.h>

namespace Err {
	struct Direct3DError:public CriticalError {
		Direct3DError(const std::string& desc):CriticalError("Direct3DError: " + desc) {}
	};
}

namespace D3D {
	enum ErrorCode {
		ErrSucceeded,
		ErrDeviceLost,
	};

	D3DFORMAT ToD3DFORMAT(Img::Format format);


	template<typename T> void ComRelease(T* obj) {
		if (obj != nullptr) {
			obj->Release();
		}
	}
}

#endif
