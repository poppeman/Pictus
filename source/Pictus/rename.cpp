#include "rename.h"
#include "res_viewer.h"
#include "ctrl_button.h"

namespace App {
	using namespace Intl;

	bool Rename::PerformOnInitDialog() {
		CreateButton(IDOK)->OnClick.connect([this]() { OnOk(); });
		CreateButton(IDCANCEL)->OnClick.connect([this]() { OnCancel(); });

		m_filename = CreateEditBox(IDC_EDIT_RENAME_NAME);
		m_filename->Filterchars(Win::EditBox::FilterInvalidFilename, SIDRenameInvalidChars);
		m_filename->Text(m_name);

		// Apply language
		Caption(SIDRename);
		ControlText(IDC_TEXT_RENAME_FILENAME, SIDRenameNewFilename);
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

	Rename::Rename(const std::wstring& name):
		Dialog{ IDD_RENAME },
		m_name( name )
	{}
}
