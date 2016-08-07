#ifndef PICTUS_HW3D_OPENGL_CONTEXT_H
#define PICTUS_HW3D_OPENGL_CONTEXT_H

#include "hw3d_context.h"
#include <memory>
#include <wx/window.h>
#include <wx/glcanvas.h>

namespace Hw3D
{
	class OpenGlContext:public Context
	{
	public:
		void Activate(wxWindow* target) override;
		void Clear(int a, int r, int g, int b) override;

		void SetTexture(int stage, std::shared_ptr<Texture> texture) override;
		void SetRenderTarget(std::shared_ptr<Texture> renderTarget) override;

		void BeginDraw() override ;
		void RenderQuad(const Vertex2D& a, const Vertex2D& b, const Vertex2D& c, const Vertex2D& d) override ;
		void EndDraw() override ;

		void SetMatrix(TransformState state, const Matrix& m) override ;

		void SendTextureRect(std::shared_ptr<Texture> sourceTexture, const Geom::RectInt& sourceRect, std::shared_ptr<Texture> destinationTexture, const Geom::PointInt& destinationTopLeft) override ;

		OpenGlContext(wxWindow* win);

	private:
		void InitializeContext();

		std::shared_ptr<wxGLContext> m_context;
		wxGLCanvas* m_currentTarget;
		GLuint m_vbo;
		GLuint m_program;
		bool m_isInitialized;
	};
}

#endif
