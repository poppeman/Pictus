#include "hw3d_opengl_context.h"

#include <GL/gl.h>

namespace Hw3D
{
	void OpenGlContext::Clear(int a, int r, int g, int b)
	{
		glClearColor(r, g, b, a);
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

}
