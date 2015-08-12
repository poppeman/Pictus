#include "StdAfx.h"
#include "dialog.h"

namespace Win {
	bool Dialog::DoModal(Win::BaseWindow* pParent) {
		Parent(pParent);
		PerformOnCreate();
		return DialogBoxParam(GetHINSTANCE(), 
						MAKEINTRESOURCE(m_iTemplateID), 
						(Parent()?Parent()->Handle():0),
						(DLGPROC)WndProcDelegate,
						(LPARAM)this) == IDOK;
	}

	bool Dialog::DoModeless(Win::BaseWindow* pParent) {
		COND_STRICT(pParent, Err::InvalidParam, TX("pParent was null"));
		if (IsModelessVisible()) {
			// Already active, send it to the foreground instead
			BringWindowToTop(Handle());
			return true;
		}
		Parent(pParent);
		RegisterWindowClasses();
		Handle(CreateDialogParam(GetHINSTANCE(), 
						MAKEINTRESOURCE(m_iTemplateID), 
						(Parent()?Parent()->Handle():0),
						(DLGPROC)WndProcDelegate,
						(LPARAM)this));
		PerformOnCreate();

		SendMessage(pParent->Handle(), WWM_REGISTERMODELESS, reinterpret_cast<WPARAM>(Handle()), 0);
		return (Handle() != 0);
	}

	Dialog::Dialog(int id):
		m_iTemplateID(id)
	{
		m_lang = Intl::OnLanguageChanged.connect([&]() { UpdateControlStrings(); });
	}

	Dialog::~Dialog() {}

	bool Dialog::OnInitDialog() { return PerformOnInitDialog(); }

	bool Dialog::OnNotify(DWORD id, LPNMHDR pnmh) { return PerformOnNotify(id, pnmh); }


	bool Dialog::GetCheckBox(DWORD id) {
		return (IsDlgButtonChecked(Handle(), id) == BST_CHECKED);
	}

	void Dialog::SetCheckBox(DWORD id, bool state) {
		CheckDlgButton(Handle(), id, (state ? BST_CHECKED : BST_UNCHECKED));
	}


	DWORD Dialog::GetEditBoxNum(DWORD id) {
		// Get string length and create a buffer to hold the string.
		char len = static_cast<char>(SendDlgItemMessage(Handle(), id, EM_LINELENGTH, 0, 0));
		boost::scoped_array<wchar_t> pC(new wchar_t[len + 1]);

		// Request the string from the edit box.
		pC[0] = static_cast<wchar_t>(len);
		SendDlgItemMessage(Handle(), id, EM_GETLINE, 0, reinterpret_cast<LPARAM>(pC.get()));
		
		// Winapi doesn't add the terminator automatically. Dummies :(		
		pC[len]	= 0;

		// Done!
		DWORD num = FromWString<DWORD>(pC.get());
		return num;
	}

	void Dialog::SetEditBoxNum(DWORD id, DWORD val) {
		SendDlgItemMessage(Handle(), id, WM_SETTEXT, 0, (LPARAM)ToWString(val).c_str());
	}

	LRESULT Dialog::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_INITDIALOG:
				return OnInitDialog();

			case WM_NOTIFY:
				return OnNotify((DWORD)wParam, (LPNMHDR)lParam);

			case WM_DESTROY:
				{
					BaseWindow* parent = Parent();
					if(parent)
						SendMessage(parent->Handle(), WWM_UNREGISTERMODELESS, reinterpret_cast<WPARAM>(Handle()), 0);
				}
				Handle(0);
				break;

			// These messages should _not_ be handled by the default handler
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MOUSEMOVE:
				return 0;
		}
		return baseWndProc(hwnd, msg, wParam, lParam, false);
	}

	void Dialog::ShowControl( DWORD id, bool doShow ) {
		ShowWindow(GetDlgItem(Handle(), id), (doShow?SW_SHOW:SW_HIDE));
	}

	void Dialog::ControlText(DWORD id, const std::wstring& text) {
		SetDlgItemText(Handle(), id, text.c_str());
	}

	void Dialog::ControlText(DWORD id, int sid) {
		m_controlSIds[id] = sid;
		ControlText(id, Intl::GetWString(sid));
	}

	void Dialog::UpdateControlStrings() {
		for(const ControlTextSIdVector::value_type& v: m_controlSIds)
			ControlText(v.first, Intl::GetWString(v.second));
	}

	bool Dialog::PerformOnNotify(DWORD id, LPNMHDR pnmh) {
		(id, pnmh); return false;
	}

	bool Dialog::PerformOnInitDialog() {
		return false;
	}

	void Dialog::PerformOnCreate() {}
	void Dialog::RegisterWindowClasses() {}
}
