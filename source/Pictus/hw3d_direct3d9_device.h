#ifndef PICTUS_HW3D_DIRECT3D9_DEVICE_H
#define PICTUS_HW3D_DIRECT3D9_DEVICE_H

#include "hw3d_device.h"

#include <d3d9.h>

namespace Hw3D
{
	class Direct3D9Device : public Device
	{
	public:
		std::shared_ptr<Context> CreateContext(wxWindow* win) override;
		std::shared_ptr<Texture> CreateTexture(const Geom::SizeInt& dimensions, Format fmt, Pool pool) override;
		std::shared_ptr<StagingTexture> CreateStagingTexture(const Geom::SizeInt& dimensions, Format fmt) override;

		bool IsLost() override;
		bool Reset() override;

		virtual Geom::SizeInt BackBufferSize() override;
		void ResizeBackBuffer(Geom::SizeInt newSize) override;

		Direct3D9Device();
		~Direct3D9Device();

	private:
		bool OnInitialize();

		D3DPRESENT_PARAMETERS m_presentParams;
		std::shared_ptr<IDirect3DDevice9Ex> m_device;
		std::shared_ptr<IDirect3D9Ex> m_d3d;

		wxWindow* m_focusWindow;
	};
}

#endif
