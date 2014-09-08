#ifndef D3D_SWAPCHAIN_H
#define D3D_SWAPCHAIN_H

#include "illa/types.h"
#include "d3d_common.h"

namespace D3D {
	class SwapChain {
	public:
		ErrorCode Present();
		Geom::SizeInt GetSize();

		LPDIRECT3DSWAPCHAIN9 D3DObject();

		SwapChain(LPDIRECT3DSWAPCHAIN9 swapChain, HWND hwnd);
		~SwapChain();

		typedef std::shared_ptr<SwapChain> Ptr;

	private:
		LPDIRECT3DSWAPCHAIN9 m_swapChain;
		HWND m_hwnd;
		Geom::SizeInt m_size;
	};
}

#endif
