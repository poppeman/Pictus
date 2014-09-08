#include "StdAfx.h"
#include "rename.h"
#include "res_viewer.h"
#include "ctrl_button.h"

namespace App {
	using namespace Intl;

	bool Rename::PerformOnInitDialog() {
		CreateButton(IDOK)->OnClick.connect([this]() { OnOk(); });
		CreateButton(IDCANCEL)->OnClick.connect([this]() { OnCancel(); });

		m_filename = CreateEditBox(IDC_EDIT_RENAME_NAME);
		m_filename->Filterchars(Win::EditBox::FilterInvalidFilename, SIDRenameInvalid);
		m_filename->Text(m_name);

		// Apply language
		Caption(SIDRenameWindow);
		ControlText(IDC_TEXT_RENAME_FILENAME, SIDRenameFilename);
		SendMessage(Handle(), WM_NEXTDLGCTL, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_RENAME_NAME), true);
		SendDlgItemMessage(Handle(), IDC_EDIT_RENAME_NAME, EM_SETSEL, 0, -1);

		// Shield icons
		//SendMessage(GetDlgItem(Handle(), IDOK), BCM_SETSHIELD, 0, TRUE);
		return false;
	}

	void Rename::OnOk() {
		std::wstring newName = m_filename->Text();
		bool ret = (newName != m_name);

		m_name = newName;
		EndDialog(Handle(), ret);
	}

	void Rename::OnCancel() {
		EndDialog(Handle(), 0);
	}

	Rename::Rename(const std::wstring& name):Dialog(IDD_RENAME), m_name(name) {}

	WNDPROC Rename::m_prevEditWndProc=0;

	LRESULT CALLBACK Rename::FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_CHAR:
			{
				// Weed out invalid characters
				char c = static_cast<uint8_t>(wParam);
				std::wstring invalid(TX("/\\:*?\")<>|"));

				if (invalid.find(c) != std::wstring::npos) {
					// Complain as well!
					std::wstring text(Intl::GetWString(SIDRenameInvalid));
					EDITBALLOONTIP bt;
					bt.cbStruct	= sizeof(bt);
					bt.pszText	= text.c_str();
					bt.pszTitle	= NULL;
					bt.ttiIcon	= TTI_NONE;
					SendMessage(hwnd, EM_SHOWBALLOONTIP, 0, (LPARAM)&bt);
					return 0;
				}

				break;
			}
		}
		return CallWindowProc(m_prevEditWndProc, hwnd, msg, wParam, lParam);
	}
}
