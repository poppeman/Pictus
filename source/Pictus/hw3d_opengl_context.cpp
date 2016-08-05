#include <GL/glew.h>
#include "hw3d_opengl_context.h"
#include "hw3d_opengl_texture.h"
#include "hw3d_opengl_common.h"

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
		glBegin(GL_TRIANGLE_STRIP); //starts drawing of points
		glVertex3f(a.Position.X, a.Position.Y, 0);
		glTexCoord2f(a.TexCoord.X, a.TexCoord.Y);
		glVertex3f(b.Position.X, b.Position.Y, 0);
		glTexCoord2f(b.TexCoord.X, b.TexCoord.Y);
		glVertex3f(c.Position.X, c.Position.Y, 0);
		glTexCoord2f(c.TexCoord.X, c.TexCoord.Y);
		glVertex3f(d.Position.X, d.Position.Y, 0);
		glTexCoord2f(d.TexCoord.X, d.TexCoord.Y);
		glEnd();

		GLenum err;
		if((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed rendering quad: " + GetGlErrorString(err));
		}
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
		auto glTexture = std::dynamic_pointer_cast<OpenGlTexture>(texture);
		if(glTexture == nullptr)
		{
			DO_THROW(Err::InvalidParam, "Texture object was not an OpenGlTexture");
		}
		glBindTexture(GL_TEXTURE_2D, glTexture->GetTextureName());
	}

	void OpenGlContext::SetRenderTarget(std::shared_ptr<Texture> renderTarget)
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
