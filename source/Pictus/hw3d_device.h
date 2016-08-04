#ifndef PICTUS_HW3D_DEVICE_H
#define PICTUS_HW3D_DEVICE_H

#include <memory>
#include <wx/window.h>

#include "hw3d_context.h"
#include "hw3d_texture.h"
#include "hw3d_vertexbuffer.h"

namespace Hw3D
{
	class Device
	{
	public:
		bool Initialize();

		virtual std::shared_ptr<Context> CreateContext(wxWindow* win)=0;
		virtual std::shared_ptr<Texture> CreateTexture(const Geom::SizeInt& dimensions, Format fmt, Pool pool)=0;
		virtual std::shared_ptr<Texture> CreateRenderTarget(const Geom::SizeInt& dimensions, Format fmt)=0;
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t sizeInBytes, int fmt)=0;

		virtual bool IsLost()=0;
		virtual bool Reset()=0;

		virtual Geom::SizeInt BackBufferSize()=0;
		virtual void ResizeBackBuffer(Geom::SizeInt newSize)=0;

		Device();
		virtual ~Device();

	private:
		virtual bool OnInitialize()=0;
	};
}

#endif
