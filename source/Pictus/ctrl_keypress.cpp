#include "ctrl_keypress.h"
#include "orz/types.h"

namespace App {
/*	LRESULT CALLBACK Keypress::FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto pCtrl = GetControl(hwnd);
		if (pCtrl == nullptr) DO_THROW(Err::CriticalError, "Couldn't find control structure.");

		auto* pEdit = dynamic_cast <Keypress*>(pCtrl.get());
		if (pEdit == nullptr) DO_THROW(Err::CriticalError, "Couldn't cast control to edit box.");

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
				KeyboardPress kp = { c, isAlt, isShift, isCtrl };
				pEdit->SetCombo(kp);

				if (c != 0 && pEdit->OnNewCombo != nullptr) {
					pEdit->OnNewCombo(kp);
				}

				return 0;
			}
			case WM_CHAR:
			{
				return 0;
			}
		}
		return CallWindowProc(pEdit->m_prevEditWndProc, hwnd, msg, wParam, lParam);
	}*/

	Keypress::Keypress(wxWindow* parent):
		wxPanel{ parent },
		OnNewCombo{ nullptr }
	{
		//m_prevEditWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Handle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(FilterEditWndProc)));
	}

	void Keypress::SetCombo(App::KeyboardPress kp) {
		std::vector<std::wstring> items;

		if (kp.Ctrl) items.push_back(L"Ctrl");
		if (kp.Shift) items.push_back(L"Shift");
		if (kp.Alt) items.push_back(L"Alt");

		if (kp.Key == WXK_CONTROL) kp.Key = 0;
		if (kp.Key == WXK_MENU) kp.Key = 0;
		std::wstring tmp;
		tmp += App::GetKeyString((wxKeyCode) kp.Key);
		items.push_back(tmp);

		auto wstr = Implode(items, L" + ");

		//SendMessage(Handle(), WM_SETTEXT, 0, (LPARAM)wstr.c_str());
	}
}
