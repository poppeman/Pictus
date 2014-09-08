#include "StdAfx.h"

#include "int_d3d_base.h"
#include "d3d_common.h"

namespace D3D {
	LPDIRECT3D9 g_d3d = 0;
	HMODULE g_dll = 0;
	int g_refcount = 0;

	typedef IDirect3D9* (__stdcall *FuncDirect3DCreate9)(UINT SDKVersion);
	FuncDirect3DCreate9 g_myDirect3DCreate = 0;

	LPDIRECT3D9 RequestDirect3D() {
		if (g_refcount == 0) {
			g_dll = ::LoadLibrary(TX("d3d9.dll"));

			if (g_dll == 0)
				return 0;

			g_myDirect3DCreate = reinterpret_cast<FuncDirect3DCreate9>(::GetProcAddress(g_dll, "Direct3DCreate9"));
			if (g_myDirect3DCreate == 0) {
				::FreeLibrary(g_dll);
				g_dll = 0;
				return 0;
			}

			g_d3d = g_myDirect3DCreate(D3D_SDK_VERSION);
			if (g_d3d == 0) {
				::FreeLibrary(g_dll);
				g_dll = 0;
				return 0;
			}
		}
		++g_refcount;

		return g_d3d;
	}

	void DropDirect3D() {
		--g_refcount;

		if (g_refcount == 0) {
			g_d3d->Release();
			g_d3d = 0;
			FreeLibrary(g_dll);
			g_dll = 0;
		}
	}
}
