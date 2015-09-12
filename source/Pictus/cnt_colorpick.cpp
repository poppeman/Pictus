#include "StdAfx.h"
#include "cnt_colorpick.h"

const wchar_t* App::ControlColorPicker::ClassName=TX("PICTUS_COLOR_PICKER");

namespace App {
	using Img::Color;

	ControlColorPicker::ControlColorPicker():
		m_hColors{ 0 },
		m_hdcMem{ CreateCompatibleDC(GetDC(0)) },
		m_colorHLS{ 0, 0, 1 },
		m_useRGB{ false }
	{
		OnMouseButtonDown.connect([this](Win::MouseEvent e) { return HandleMouseClick(e); });
		OnMouseMove.connect([this](Win::MouseEvent e) { return HandleMouseMove(e); });
	}

	ControlColorPicker::~ControlColorPicker() {
		if (m_hColors) DeleteObject(m_hColors);
		DeleteDC(m_hdcMem);
	}


	void ControlColorPicker::RegisterClass(HINSTANCE hInstance) {
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

	LRESULT ControlColorPicker::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_CREATE:
				CreateColorBitmap();
				break;
			case WM_PAINT:
				{
					PAINTSTRUCT ps;

					HDC hdc=BeginPaint(hwnd, &ps);
					HGDIOBJ hobj=SelectObject(m_hdcMem, m_hColors);

					int w=ps.rcPaint.right-ps.rcPaint.left;
					int h=ps.rcPaint.bottom-ps.rcPaint.top;

					BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, w, h, m_hdcMem, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
					SelectObject(m_hdcMem, hobj);

					// Draw cursor
					Geom::PointInt m_pos((int)(m_colorHLS.H*(w/360.0f)),  (int)(m_colorHLS.L*h));
					for (int x=0;x<CursorLength;x++) SetPixel(hdc, x+m_pos.X-CursorSpace-CursorLength, m_pos.Y, RGB(255, 255, 0));
					for (int x=0;x<CursorLength;x++) SetPixel(hdc, 1+x+m_pos.X+CursorSpace, m_pos.Y, RGB(255, 255, 0));
					for (int y=0;y<CursorLength;y++) SetPixel(hdc, m_pos.X, y+m_pos.Y-CursorSpace-CursorLength, RGB(255, 255, 0));
					for (int y=0;y<CursorLength;y++) SetPixel(hdc, m_pos.X, 1+y+m_pos.Y+CursorSpace, RGB(255, 255, 0));

					EndPaint(hwnd, &ps);
					return 0;
				}

			case MsgSetHue:			// Change hue
				m_useRGB		= false;
				m_colorHLS.H	= static_cast<float>(lParam);
				InvalidateRect(Handle(), 0, false);
				notifyChange();
				return 0;

			case MsgSetLuminance:	// Change luminance
				m_useRGB		= false;
				m_colorHLS.L	= lParam/100.0f;
				InvalidateRect(Handle(), 0, false);
				notifyChange();
				return 0;

			case MsgSetSaturation:	// Change saturation
				m_useRGB=false;
				m_colorHLS.S=lParam/100.0f;;
				CreateColorBitmap();
				InvalidateRect(Handle(), 0, false);
				notifyChange();
				return 0;

			case MsgSetRGB:			// Set RGB color
				{
					SetRGB(static_cast<DWORD>(wParam));
					InvalidateRect(Handle(), 0, false);
					notifyChange();
					return 0;
			   }
			case MsgGetRGB:	// Get RGB color
				return GetRGB();

			case MsgGetHue:
				return static_cast<LRESULT>(m_colorHLS.H);

			case MsgGetLuminance:
				return static_cast<LRESULT>((m_colorHLS.L + 0.005f) * 100.0f);

			case MsgGetSaturation:
				return static_cast<LRESULT>((m_colorHLS.S + 0.005f) * 100.0f);
		}
		return baseWndProc(hwnd, msg, wParam, lParam, true);
	}

	bool ControlColorPicker::HandleMouseClick(Win::MouseEvent e) {
		SetCursorPosition(e.Position);
		return true;
	}

	bool ControlColorPicker::HandleMouseMove(Win::MouseEvent e) {
		if (e.Button == Win::Left)
			SetCursorPosition(e.Position);

		return true;
	}

	void ControlColorPicker::CreateColorBitmap() {
		if (m_hColors) DeleteObject(m_hColors);

		int w = Width();
		int h = Height();
		std::vector<DWORD> pData(w*h);

		int index = 0;
		for (int y = 0; y < h; ++y)
			for (int x = 0; x < w; ++x) {
				pData[index++] = ToARGBDWORD(Img::HLS_RGB(
					static_cast<float>(Img::HueCap) * x / w,
					static_cast<float>(Img::LumCap) * y / h,
					m_colorHLS.S));
			}

		// Initialize bitmap
		BITMAPINFO bi;
		ZeroMemory(&bi, sizeof(bi));
		bi.bmiHeader.biSize			= sizeof(bi.bmiHeader);
		bi.bmiHeader.biBitCount		= 32;
		bi.bmiHeader.biCompression	= BI_RGB;
		bi.bmiHeader.biWidth		= w;
		bi.bmiHeader.biHeight		= -h;
		bi.bmiHeader.biPlanes		= 1;
		bi.bmiHeader.biSizeImage	= 0;

		m_hColors=CreateDIBitmap(GetDC(0), &bi.bmiHeader, CBM_INIT, (void*)&pData[0], &bi, 0);
	}

	void ControlColorPicker::SetCursorPosition(const Geom::PointInt& p) {
		InvalidateRect(Handle(), 0, false);

		int w = Width();
		int h = Height();

		m_useRGB = false;

		// Calculate hue and luminance from pixel values
		m_colorHLS.H = static_cast<float>(Img::HueCap * p.X / w);
		m_colorHLS.L = static_cast<float>(Img::LumCap * p.Y / h);

		notifyChange();
	}

	void ControlColorPicker::SetRGB(DWORD col) {
		m_colorRGB	= Img::Color::FromDWord(col);
		m_useRGB	= true;
		m_colorHLS	= Img::RGB_HLS(
			m_colorRGB.R,
			m_colorRGB.G,
			m_colorRGB.B);
		CreateColorBitmap();	// Needs to be updated (saturation might have been changed)
	}

	DWORD ControlColorPicker::GetRGB() {
		if (m_useRGB)
			return ToARGBDWORD(m_colorRGB);

		return ToARGBDWORD(Img::HLS_RGB(m_colorHLS));
	}

	void ControlColorPicker::notifyChange() {
		SendMessage(::GetParent(Handle()), WM_APP+NotValueChanged, 0, 0);
	}

	LRESULT WINAPI ControlColorPicker::wndProc_delegate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (msg == WM_NCCREATE) {
			ControlColorPicker* pWin = new ControlColorPicker();
			SetProp(hwnd, ClassPtrProp, reinterpret_cast<HANDLE>(pWin));
			pWin->Handle(hwnd);
		}

		ControlColorPicker* pWin = reinterpret_cast<ControlColorPicker*>(GetProp(hwnd, ClassPtrProp));

		if (msg == WM_DESTROY) {
			delete pWin;
			EnumProps(hwnd, BaseWindow::DelPropProc);
		}
		else if (pWin)
			return pWin->wndProc(hwnd, msg, wParam, lParam);

		return false;
	}
}
