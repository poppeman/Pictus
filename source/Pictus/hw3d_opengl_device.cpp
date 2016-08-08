#include "hw3d_opengl_device.h"
#include "hw3d_opengl_context.h"
#include "hw3d_opengl_texture.h"
#include "hw3d_opengl_staging_texture_pbo.h"
#include "hw3d_opengl_staging_texture_simple.h"

namespace Hw3D
{
	std::shared_ptr<Texture> OpenGlDevice::CreateTexture(const Geom::SizeInt &dimensions, Format fmt, Pool pool)
	{
		return std::make_shared<OpenGlTexture>(dimensions, fmt, pool);
	}

	std::shared_ptr<Texture> OpenGlDevice::CreateRenderTarget(const Geom::SizeInt &dimensions, Format fmt)
	{
		return std::make_shared<OpenGlTexture>(dimensions, fmt, Pool::Default);
	}

	std::shared_ptr<StagingTexture> OpenGlDevice::CreateStagingTexture(const Geom::SizeInt& dimensions, Format fmt)
	{
		if(GLEW_ARB_pixel_buffer_object)
		{
			return std::make_shared<OpenGlStagingTexturePbo>(dimensions, fmt);
		}
		else
		{
			return std::make_shared<OpenGlStagingTextureSimple>(dimensions, fmt);
		}
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
