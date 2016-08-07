#include "hw3d_opengl_device.h"
#include "hw3d_opengl_context.h"
#include "hw3d_opengl_texture_pbo.h"
#include "hw3d_opengl_texture_simple.h"

namespace Hw3D
{
	std::shared_ptr<Texture> OpenGlDevice::CreateTexture(const Geom::SizeInt &dimensions, Format fmt, Pool pool)
	{
		if(GLEW_ARB_pixel_buffer_object && false)
		{
			return std::make_shared<OpenGlTexturePbo>(dimensions, fmt, pool);
		}
		else
		{
			return std::make_shared<OpenGlTextureSimple>(dimensions, fmt, pool);
		}
	}

	std::shared_ptr<Texture> OpenGlDevice::CreateRenderTarget(const Geom::SizeInt &dimensions, Format fmt)
	{
		return std::make_shared<OpenGlTexturePbo>(dimensions, fmt, Pool::Default);
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
