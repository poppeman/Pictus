#include "ctrl_editbox.h"
#include "orz/exception.h"
#include "orz/types.h"
#include <boost/scoped_array.hpp>

#include <CommCtrl.h>

namespace Win {
	void EditBox::Filterchars(FilterCriteria criteria, int sidBalloonMessage) {
		// Disable current filter
		if (m_prevEditWndProc) {
			SetWindowLongPtr(Handle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevEditWndProc));
			m_prevEditWndProc = 0;
		}

		// Store string and stuff
		m_balloonText = sidBalloonMessage;
		m_criteria = criteria;

		// Subclass the control with the new filter (unless it's no filtering)
		if (m_criteria == FilterNone) return;

		m_prevEditWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Handle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(FilterEditWndProc)));
	}

	std::wstring EditBox::Text() {
		// Prepare a buffer for the string
		int len = GetWindowTextLength(Handle()) + 1;
		boost::scoped_array<wchar_t> str(new wchar_t[len]);

		// Fetch string
		GetWindowText(Handle(), str.get(), len);

		// Wrap it up in a pretty string and return it
		std::wstring ret(str.get());
		return ret;
	}

	void EditBox::Text(const std::wstring& s) {
		SendMessage(Handle(), WM_SETTEXT, 0, (LPARAM)s.c_str());
	}

	LRESULT CALLBACK EditBox::FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		Control::Ptr pCtrl = GetControl(hwnd);
		COND_STRICT(pCtrl, Err::CriticalError, TX("Couldn't find control structure."));

		EditBox* pEdit = dynamic_cast <EditBox*>(pCtrl.get());
		COND_STRICT(pEdit, Err::CriticalError, TX("Couldn't cast control to edit box."));

		switch (msg) {
			case WM_PASTE:
				{
					if (!::OpenClipboard(hwnd)) break;
					if (::IsClipboardFormatAvailable(CF_UNICODETEXT)) {
						HANDLE hData = GetClipboardData(CF_UNICODETEXT);
						if (hData != 0) {
							wchar_t* pStr = reinterpret_cast<wchar_t*>(GlobalLock(hData));
							if (pStr != 0) {
								bool someInvalid = false;

								// Send each valid character through a WM_CHAR message.
								size_t len = wcslen(pStr);
								for (size_t i = 0; i < len; i++) {
									if (pEdit->isValidchar(pStr[i]) == true) {
										// Valid character, send it.
										SendMessage(hwnd, WM_CHAR, pStr[i], 0);
									}
									else {
										// Invalid, flag an error.
										someInvalid = true;
									}
								}
								GlobalUnlock(hData); 

								// Show a balloon tip if there were an error.
								if (someInvalid) pEdit->showBalloon(Intl::GetWString(pEdit->m_balloonText));
							}
						}
					}
					CloseClipboard(); 
					return 0;
				}

			case WM_CHAR:
				{
					char c = (char)wParam;

					if (!pEdit->isValidchar(c)) {
						pEdit->showBalloon(Intl::GetWString(pEdit->m_balloonText));
						return 0;
					}

					break;
				}
		}
		return CallWindowProc(pEdit->m_prevEditWndProc, hwnd, msg, wParam, lParam);
	}

	EditBox::EditBox(int id, HWND hwnd):
		Control{ id, hwnd },
		m_balloonText{ -1 },
		m_criteria{ FilterNone },
		m_prevEditWndProc{ 0 }
	{}

	bool EditBox::isValidchar(char c) {
		bool isValid = true;

		switch(m_criteria) {
			case FilterInvalidFilename:
				{
					std::wstring invalid(TX("/\\:*?\"<>|"));
					isValid = invalid.find(c) == std::wstring::npos;
				}
				break;

			case FilterNotNumerical:
				{
					if ((c != 0x8) && !(c >= TX('0') && c <= TX('9'))) isValid = false;
				}
				break;
		}
		return isValid;
	}

	void EditBox::showBalloon(const std::wstring& str) {
		if (str != TX("")) {
			EDITBALLOONTIP bt;
			bt.cbStruct = sizeof(bt);
			bt.pszText  = str.c_str();
			bt.pszTitle = NULL;
			bt.ttiIcon = TTI_NONE;
			SendMessage(Handle(), EM_SHOWBALLOONTIP, 0, (LPARAM)&bt);
		}
	}
}
