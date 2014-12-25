#include "StdAfx.h"

#include "d3d_vertexbuffer.h"
#include "d3d_common.h"

namespace D3D {
	VertexBuffer::VertexBuffer(LPDIRECT3DVERTEXBUFFER9 vb, DWORD fvf, size_t bytes) :
		m_vb{ vb, ComRelease < IDirect3DVertexBuffer9 > },
		m_isLocked(false),
		m_fvf(fvf),
		m_sizeInBytes(bytes)
	{
		if (vb == 0) {
			DO_THROW(Err::CriticalError, TX("Vertex buffer not initialized"));
		}
	}

	uint8_t* VertexBuffer::Lock() {
		if (m_isLocked) {
			DO_THROW(Err::CriticalError, TX("Vertex buffer is already locked."));
		}
		void* buf;
		m_vb->Lock(0, 0, &buf, 0);
		m_isLocked = true;
		return reinterpret_cast<uint8_t*>(buf);
	}

	void VertexBuffer::Unlock() {
		if (m_isLocked == false) {
			DO_THROW(Err::CriticalError, TX("Vertex buffer is not locked."));
		}
		m_vb->Unlock();
		m_isLocked = false;
	}

	size_t VertexBuffer::SizeInBytes() const {
		return m_sizeInBytes;
	}

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer::D3DObject() {
		return m_vb.get();
	}

	DWORD VertexBuffer::FVFDefinition() {
		return m_fvf;
	}
}
