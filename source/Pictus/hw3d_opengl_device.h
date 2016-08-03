#ifndef PICTUS_HW3D_OPENGL_DEVICE_H
#define PICTUS_HW3D_OPENGL_DEVICE_H

#include "hw3d_device.h"

#ifdef _WIN32
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

namespace Hw3D
{
	class OpenGlDevice : public Device
	{
	public:
		std::shared_ptr<Context> CreateContext() override;
		std::shared_ptr<Texture> CreateTexture(const Geom::SizeInt& dimensions, Format fmt, Pool pool) override;
		std::shared_ptr<Texture> CreateRenderTarget(const Geom::SizeInt& dimensions, Format fmt) override;
		std::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t sizeInBytes, int fmt) override;

		bool IsLost() override;
		bool Reset() override;

		virtual Geom::SizeInt BackBufferSize() override;
		void ResizeBackBuffer(Geom::SizeInt newSize) override;

		OpenGlDevice();
		~OpenGlDevice();

	private:
		bool OnInitialize();
	};
}

#endif
