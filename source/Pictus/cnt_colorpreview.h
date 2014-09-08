
#ifndef CNT_COLORPREVIEW_H
#define CNT_COLORPREVIEW_H

#include "dialog.h"

namespace App {
	// TODO: Make DPI aware
	class ControlColorPreview:public Win::BaseWindow {
	public:
		static const wchar_t* ClassName;

		enum Messages {
			MsgSetRGB=WM_APP+1,
		};

		static void RegisterClass(HINSTANCE hInstance);
		LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		ControlColorPreview();

	private:
		DWORD m_col;

		static LRESULT WINAPI wndProc_delegate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif

