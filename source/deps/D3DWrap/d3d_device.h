#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "d3d_swapchain.h"
#include "d3d_texture.h" 
#include "d3d_vertexbuffer.h"

#define D3D_VERTEX_FVF D3DFVF_XYZ | D3DFVF_TEX1

namespace D3D {
	struct vbVertex {
		float x, y, z;
		float u, v;
	};

	class Device {
	public:
		bool Initialize(HWND hwndFocus, bool doWindowed = true, int device = 0);
		void ResizeBackBuffer(const Geom::SizeInt& newSize);

		void Clear(int a, int r, int g, int b);

		SwapChain::Ptr CreateSwapChain(HWND hwnd);
		Texture::Ptr CreateTexture(const Geom::SizeInt& dimensions, D3DFORMAT fmt, D3DPOOL pool);
		Texture::Ptr CreateRenderTarget(const Geom::SizeInt& dimensions, D3DFORMAT fmt);
		VertexBuffer::Ptr CreateVertexBuffer(size_t sizeInBytes, int fmt);

		void SetTexture(int stage, Texture::Ptr texture);
		void SetSwapChain(SwapChain::Ptr swapChain);
		void SetRenderTarget(Texture::Ptr renderTarget);
		void SetVertexBuffer(VertexBuffer::Ptr vb, int stride);

		void ResampleFilter(DWORD stage, D3DTEXTUREFILTERTYPE filter);

		bool IsLost();
		bool Reset();

		void BeginDraw();
		void RenderQuad(const Vertex2D& a, const Vertex2D& b, const Vertex2D& c, const Vertex2D& d);
		void RenderQuad(int index);
		void EndDraw();

		void Present();
		void SetMatrix(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix);

		const Geom::SizeInt MaxTextureSize() const;

		void SendTextureRect(D3D::Texture::Ptr sourceTexture, const Geom::RectInt& sourceRect, D3D::Texture::Ptr destinationTexture, const Geom::PointInt& destinationTopLeft);

		Device();
		~Device();

		typedef std::shared_ptr<Device> Ptr;

	private:
		void Release();

		LPDIRECT3D9 m_d3d;
		LPDIRECT3DDEVICE9 m_device;

		D3DPRESENT_PARAMETERS m_presentParams;
		bool m_isDrawing;

		SwapChain::Ptr m_currentSwapChain;
	};
}

#endif
