#ifndef HW3D_OGL_DEVICE_H
#define HW3D_OGL_DEVICE_H

#include <wx/window.h>
#include <memory>
#include "matrix.h"
#include "ogl_texture.h"
#include "ogl_vertexbuffer.h"
#include "types.h"

namespace Hw3D {
	class Device {
	public:
		bool Initialize(wxWindow* target);
		void Clear(int a, int r, int g, int b);

		std::shared_ptr<Texture> CreateTexture(const Geom::SizeInt& dimensions, Format fmt, Pool pool);
		std::shared_ptr<Texture> CreateRenderTarget(const Geom::SizeInt& dimensions, Format fmt);
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t sizeInBytes, int fmt);

		bool IsLost();
		bool Reset();

		void BeginDraw();
		void RenderQuad(const Vertex2D& a, const Vertex2D& b, const Vertex2D& c, const Vertex2D& d);
		void RenderQuad(int index);
		void EndDraw();

		void SetTexture(int stage, std::shared_ptr<Texture> texture);
		void SetRenderTarget(std::shared_ptr<Texture> renderTarget);
		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vb, int stride);

		void SetMatrix(TransformState state, const Matrix& m);

		Geom::SizeInt BackBufferSize();
		void ResizeBackBuffer(Geom::SizeInt newSize);

		void SendTextureRect(std::shared_ptr<Texture> sourceTexture, const Geom::RectInt& sourceRect, std::shared_ptr<Texture> destinationTexture, const Geom::PointInt& destinationTopLeft);
	};
}

#endif

