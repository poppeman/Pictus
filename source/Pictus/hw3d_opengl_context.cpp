#include <GL/glew.h>
#include "hw3d_opengl_context.h"

#include <GL/gl.h>
#include <wx/glcanvas.h>
#include <orz/exception.h>

namespace Hw3D
{
	void OpenGlContext::Activate(wxWindow *target)
	{
		if(target == nullptr)
		{
			DO_THROW(Err::CriticalError, "OpenGlContext::Activate: target was null");
		}
		m_currentTarget = dynamic_cast<wxGLCanvas*>(target);
		if(m_currentTarget == nullptr)
		{
			DO_THROW(Err::CriticalError, "OpenGlContext::Activate: target was not a wxGLCanvas object");
		}

		m_context->SetCurrent(*m_currentTarget);

		if (glewInit() != GLEW_OK)
		{
			DO_THROW(Err::CriticalError, "Failed initializing GLEW");
		}
	}

	void OpenGlContext::Clear(int a, int r, int g, int b)
	{
		glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlContext::BeginDraw()
	{
	}

	void OpenGlContext::RenderQuad(const Vertex2D &a, const Vertex2D &b, const Vertex2D &c, const Vertex2D &d)
	{
	}

	void OpenGlContext::RenderQuad(int index)
	{

	}

	void OpenGlContext::EndDraw()
	{
		glFlush();
		m_currentTarget->SwapBuffers();
	}

	void OpenGlContext::SetTexture(int stage, std::shared_ptr<Texture> texture)
	{

	}

	void OpenGlContext::SetRenderTarget(std::shared_ptr<Texture> renderTarget)
	{

	}

	void OpenGlContext::SetVertexBuffer(std::shared_ptr<VertexBuffer> vb, int stride)
	{

	}

	void OpenGlContext::SetMatrix(TransformState state, const Matrix &m)
	{

	}

	void OpenGlContext::SendTextureRect(std::shared_ptr<Texture> sourceTexture, const Geom::RectInt &sourceRect,
										std::shared_ptr<Texture> destinationTexture,
										const Geom::PointInt &destinationTopLeft)
	{

	}

	OpenGlContext::OpenGlContext(wxWindow *win):
		m_context{std::make_shared<wxGLContext>(dynamic_cast<wxGLCanvas*>(win))}
	{
	}
}
