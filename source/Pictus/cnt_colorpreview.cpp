#include "cnt_colorpreview.h"

const wchar_t* App::ControlColorPreview::ClassName = L"PICTUS_COLOR_PREVIEW";

namespace App {
	void ControlColorPreview::RegisterClass(HINSTANCE hInstance) {
		// Create custom class for the color picker
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));

		wc.cbSize         = sizeof(wc);
		wc.lpszClassName  = ClassName;
		wc.hInstance      = hInstance;
		wc.lpfnWndProc    = wndProc_delegate;
		wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground  = (HBRUSH)GetSysColorBrush(WHITE_BRUSH);
		RegisterClassEx(&wc);
	}

	LRESULT ControlColorPreview::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg)
		{
			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc=BeginPaint(hwnd, &ps);
					HBRUSH brush=CreateSolidBrush(m_col);
					FillRect(hdc, &ps.rcPaint, brush);
					DeleteObject(brush);
					EndPaint(hwnd, &ps);
					return 0;
				}

			case MsgSetRGB:		// Change color
				m_col = Img::ABGR_ARGB(static_cast<uint32_t>(lParam & 0xffffffff));
				InvalidateRect(hwnd, 0, false);
				return 0;
		}
		return baseWndProc(hwnd, msg, wParam, lParam, true);
	}

	ControlColorPreview::ControlColorPreview():
		m_col{ 0xff9275 }
	{}

	LRESULT WINAPI ControlColorPreview::wndProc_delegate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (msg == WM_NCCREATE) {
			ControlColorPreview* pWin=new ControlColorPreview();
			pWin->Handle(hwnd);
			SetProp(hwnd, ClassPtrProp, reinterpret_cast<HANDLE>(pWin));
		}

		ControlColorPreview* pWin = reinterpret_cast<ControlColorPreview*>(GetProp(hwnd, ClassPtrProp));

		if (msg == WM_DESTROY) {
			delete pWin;
			EnumProps(hwnd, BaseWindow::DelPropProc);
		}
		else if (pWin)
			return pWin->wndProc(hwnd, msg, wParam, lParam);

		return false;
	}
}
