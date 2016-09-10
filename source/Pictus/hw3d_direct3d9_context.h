#ifndef HW3D_DIRECT3D9_CONTEXT_H
#define HW3D_DIRECT3D9_CONTEXT_H

#include "hw3d_context.h"
#include <d3d9.h>

namespace Hw3D
{
	class Direct3D9Context : public Context
	{
	public:
		void Activate(wxWindow* target) override;
		void Clear(int a, int r, int g, int b) override;

		void SetTexture(int stage, std::shared_ptr<Texture> texture) override;

		void BeginDraw() override;
		void RenderQuad(const Vertex2D& a, const Vertex2D& b, const Vertex2D& c, const Vertex2D& d) override;
		void EndDraw() override;

		void SetMatrix(TransformState state, const Matrix& m) override;

		void SendTextureRect(std::shared_ptr<StagingTexture> sourceTexture, const Geom::RectInt& sourceRect, std::shared_ptr<Texture> destinationTexture, const Geom::PointInt& destinationTopLeft) override;

		Direct3D9Context(wxWindow* win, std::shared_ptr<IDirect3DDevice9Ex> dev);

	private:
		bool m_isInitialized;
		bool m_isDrawing;

		std::shared_ptr<IDirect3DDevice9Ex> m_device;
		wxWindow* m_currentTarget;
	};
}

#endif
