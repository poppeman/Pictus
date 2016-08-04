#include "hw3d_opengl_device.h"
#include "hw3d_opengl_context.h"
#include "hw3d_opengl_texture.h"
#include "hw3d_opengl_vertexbuffer.h"
#include <GL/gl.h>
#include <GL/glx.h>

namespace Hw3D
{
	std::shared_ptr<Texture> OpenGlDevice::CreateTexture(const Geom::SizeInt &dimensions, Format fmt, Pool pool)
	{
		return std::shared_ptr<OpenGlTexture>();
	}

	std::shared_ptr<Texture> OpenGlDevice::CreateRenderTarget(const Geom::SizeInt &dimensions, Format fmt)
	{
		return std::shared_ptr<OpenGlTexture>();
	}

	std::shared_ptr<VertexBuffer> OpenGlDevice::CreateVertexBuffer(size_t sizeInBytes, int fmt)
	{
		return std::shared_ptr<OpenGlVertexBuffer>();
	}

	bool OpenGlDevice::IsLost()
	{
		return false;
	}

	bool OpenGlDevice::Reset()
	{
		return false;
	}

	Geom::SizeInt OpenGlDevice::BackBufferSize()
	{
		return Geom::Size<int>();
	}

	void OpenGlDevice::ResizeBackBuffer(Geom::SizeInt newSize)
	{

	}

	OpenGlDevice::OpenGlDevice()
	{
	}

	OpenGlDevice::~OpenGlDevice()
	{
	}

	bool OpenGlDevice::OnInitialize()
	{
		return false;
	}

	std::shared_ptr<Context> OpenGlDevice::CreateContext(wxWindow* win)
	{
		return std::make_shared<OpenGlContext>(win);
	}
}
