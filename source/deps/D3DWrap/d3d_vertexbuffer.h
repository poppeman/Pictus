#ifndef D3DWRAP_D3D_VERTEXBUFFER_H
#define D3DWRAP_D3D_VERTEXBUFFER_H

#include "illa/types.h"

#include <memory>

#include <d3d9.h>

namespace D3D {
	enum VertexFormat {
		VFPositionXYZ = D3DFVF_XYZ,
		VFTex01 = D3DFVF_TEX1,
	};

	class VertexBuffer {
	public:
		VertexBuffer(LPDIRECT3DVERTEXBUFFER9 vb, DWORD fvf, size_t bytes);

		uint8_t* Lock();
		void Unlock();

		size_t SizeInBytes() const;

		LPDIRECT3DVERTEXBUFFER9 D3DObject();
		DWORD FVFDefinition();

		typedef std::shared_ptr<VertexBuffer> Ptr;

	private:
		std::shared_ptr<IDirect3DVertexBuffer9> m_vb;
		bool m_isLocked;
		size_t m_sizeInBytes;
		DWORD m_fvf;
	};
}

#endif
