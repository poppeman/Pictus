#include "StdAfx.h"

#include "d3d_swapchain.h"
#include "d3d_common.h"
#include "int_d3d_types.h"

namespace D3D {
	ErrorCode SwapChain::Present() {
		HRESULT hret = m_swapChain->Present(0, 0, 0, 0, 0);
		if (hret == D3DERR_DEVICELOST) return ErrDeviceLost;
		if (hret != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("An unknown error occured."));
		}
		ValidateRect(m_hwnd, 0);
		return ErrSucceeded;
	}

	Geom::SizeInt SwapChain::GetSize() {
		return m_size;
	}

	SwapChain::SwapChain(LPDIRECT3DSWAPCHAIN9 swapChain, HWND hwnd):
		m_swapChain{ swapChain },
		m_hwnd{ hwnd }
	{
		RECT rect;
		::GetClientRect(hwnd, &rect);
		m_size = RECTToRect(rect).Dimensions();
	}

	SwapChain::~SwapChain() {
		SAFE_RELEASE(m_swapChain);
	}

	LPDIRECT3DSWAPCHAIN9 SwapChain::D3DObject() {
		if (m_swapChain == 0) {
			DO_THROW(Err::CriticalError, TX("Object not yet created."));
		}
		return m_swapChain;
	}
}
