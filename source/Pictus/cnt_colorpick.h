#ifndef CNT_COLORPICK_H
#define CNT_COLORPICK_H

#include "dialog.h"

namespace App {
	// TODO: Make DPI aware
	class ControlColorPicker:public Win::BaseWindow {
	public:
		static const wchar_t*		ClassName;

		enum {
			CursorSpace=2,
			CursorLength=6,
		};

		enum Messages {
			MsgSetHue = WM_APP+1,
			MsgSetLuminance,
			MsgSetSaturation,

			MsgSetRGB,
			MsgGetRGB,

			MsgGetHue,
			MsgGetLuminance,
			MsgGetSaturation,
		};

		enum Notifications {
			NotValueChanged=1
		};

	public:
		static void RegisterClass(HINSTANCE hInstance);
		LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		ControlColorPicker();
		~ControlColorPicker();

	private:
		bool HandleMouseMove(Win::MouseEvent e);
		bool HandleMouseClick(Win::MouseEvent e);

		void CreateColorBitmap();

		void SetCursorPosition(const Geom::PointInt& pos);
		void SetRGB(DWORD col);

		DWORD GetRGB();

		void notifyChange();

		// Color in HLS and RGB version
		Img::HLSTriplet m_colorHLS;
		Img::Color m_colorRGB;

		bool m_useRGB;

		// Palette bitmap
		HBITMAP m_hColors;
		HDC m_hdcMem;

	private:
		static LRESULT WINAPI wndProc_delegate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif
