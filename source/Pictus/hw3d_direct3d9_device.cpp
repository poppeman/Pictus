#include "hw3d_direct3d9_device.h"
#include "hw3d_direct3d9_context.h"
#include "hw3d_direct3d9_texture.h"
#include "hw3d_direct3d9_staging_texture.h"
#include "orz\exception.h"
#include "hw3d_direct3d9_common.h"

namespace Hw3D
{
	template<typename T> ::std::shared_ptr<T> WrapRelease(T* obj)
	{
		return ::std::shared_ptr<T>(obj, ComRelease<T>);
	}

	std::shared_ptr<Context> Direct3D9Device::CreateContext(wxWindow* win)
	{
		m_focusWindow = win;
		OnInitialize();
		return std::make_shared<Direct3D9Context>(win, m_device);
	}

	std::shared_ptr<Texture> Direct3D9Device::CreateTexture(const Geom::SizeInt &dimensions, Format fmt, Pool pool)
	{
		if (m_device == nullptr)
		{
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		LPDIRECT3DTEXTURE9 texture;

		if (m_device->CreateTexture(dimensions.Width, dimensions.Height, 1, 0, ConvertFormat(fmt), ConvertPool(pool), &texture, 0) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "Couldn't allocate texture.");
		}

		return std::make_shared<Direct3D9Texture>(texture, dimensions, fmt);
	}

	std::shared_ptr<StagingTexture> Direct3D9Device::CreateStagingTexture(const Geom::SizeInt& dimensions, Format fmt)
	{
		if (m_device == nullptr)
		{
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		LPDIRECT3DTEXTURE9 texture;

		if (m_device->CreateTexture(dimensions.Width, dimensions.Height, 1, 0, ConvertFormat(fmt), ConvertPool(Pool::SystemMemory), &texture, 0) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "Couldn't allocate texture.");
		}

		return std::make_shared<Direct3D9StagingTexture>(texture, dimensions, fmt);
	}

	bool Direct3D9Device::IsLost()
	{
		return (m_device->TestCooperativeLevel() != D3D_OK);
	}

	bool Direct3D9Device::Reset()
	{
		HRESULT hret = m_device->ResetEx(&m_presentParams, nullptr);
		return (hret == D3D_OK);
	}

	Geom::SizeInt Direct3D9Device::BackBufferSize()
	{
		return Geom::Size<int>();
	}

	void Direct3D9Device::ResizeBackBuffer(Geom::SizeInt newSize)
	{
		m_presentParams.BackBufferWidth = m_focusWindow->GetSize().GetWidth();
		m_presentParams.BackBufferHeight = m_focusWindow->GetSize().GetHeight();

		if (m_device->ResetEx(&m_presentParams, nullptr) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "Failed setting backbuffer size");
		}
	}

	Direct3D9Device::Direct3D9Device()
	{
		IDirect3D9Ex* pdex = 0;
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &pdex))) {
			pdex = 0;
		}

		m_d3d = WrapRelease(pdex);
	}

	Direct3D9Device::~Direct3D9Device()
	{
	}

	bool Direct3D9Device::OnInitialize()
	{
		// Can occur of Direct3D9 is not installed.
		if (m_d3d == 0)
		{
			DO_THROW(Err::CriticalError, "Direct3D 9 could not be found");
		}

		ZeroMemory(&m_presentParams, sizeof(m_presentParams));

		m_presentParams.Windowed = true;
		m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		//m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		//m_presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		//m_presentParams.hDeviceWindow = hwndFocus;
		m_presentParams.BackBufferCount = 1;

		m_presentParams.BackBufferFormat = D3DFMT_UNKNOWN;
		m_presentParams.BackBufferWidth = 640;
		m_presentParams.BackBufferHeight = 480;

		IDirect3DDevice9Ex* pdev = nullptr;
		auto hr = m_d3d->CreateDeviceEx(
			0,
			D3DDEVTYPE_HAL,
			m_focusWindow->GetHandle(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_presentParams,
			nullptr,
			&pdev);

		switch (hr)
		{
		case D3DERR_DEVICELOST:
			DO_THROW(Err::CriticalError, "Direct3D Device was lost.");
		case D3DERR_INVALIDCALL:
			DO_THROW(Err::CriticalError, "Invalid call during CreateDevice.");
		case D3DERR_NOTAVAILABLE:
			DO_THROW(Err::CriticalError, "Direct3D device does not support required.");
		case D3DERR_OUTOFVIDEOMEMORY:
			DO_THROW(Err::CriticalError, "Not enough video memory to initialize Direct3D.");
		}

		m_device = WrapRelease(pdev);

		if (m_device == 0)
			return false;

		return true;
	}
}
