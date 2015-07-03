#include "StdAfx.h"
#include "ctrl_keypress.h"

namespace App {
	LRESULT CALLBACK Keypress::FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto pCtrl = GetControl(hwnd);
		if (pCtrl == nullptr) DO_THROW(Err::CriticalError, TX("Couldn't find control structure."));

		auto* pEdit = dynamic_cast <Keypress*>(pCtrl.get());
		if (pEdit == nullptr) DO_THROW(Err::CriticalError, TX("Couldn't cast control to edit box."));

		switch (msg) {
			case WM_PASTE:
			{
				return 0;
			}

			case WM_SYSKEYDOWN: // Allows interception ALT-related key presses
			case WM_KEYDOWN:
			{
				wchar_t c = (wchar_t)wParam;

				// 0x8000 = Pressed bit, ignoring toggle bit
				auto isCtrl = GetKeyState(VK_CONTROL) & 0x8000;
				auto isShift = GetKeyState(VK_SHIFT) & 0x8000;
				auto isAlt = GetKeyState(VK_MENU) & 0x8000;

				std::wstring wstr;
				if (isCtrl) wstr += L"Ctrl";
				if (isShift) wstr += L"Shift";
				if (isAlt) wstr += L"Alt";

				if (c == VK_CONTROL) c = L'';
				if (c == VK_MENU) c = L'';
				wstr += L" + ";

				wstr += c;

				SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)wstr.c_str());
				return 0;
			}
			case WM_CHAR:
			{
				return 0;

				wchar_t c = (wchar_t)wParam;
				std::wstring wstr;
				wstr += c;

				SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)wstr.c_str());
				return 0;
			}
		}
		return CallWindowProc(pEdit->m_prevEditWndProc, hwnd, msg, wParam, lParam);
	}

	Keypress::Keypress(int id, HWND hwnd):
		Control{ id, hwnd },
		m_prevEditWndProc{ nullptr } {

		m_prevEditWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Handle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(FilterEditWndProc)));
	}

	Keypress* Keypress::CreateKeypress(int id, HWND parent) {
		auto p = Control::GetControl(GetDlgItem(parent, id));
		if (p != nullptr) {
			return dynamic_cast<Keypress*>(p.get());
		}

		return new Keypress(id, parent);
	}
}
