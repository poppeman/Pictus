#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_keyboard.h"

#include "ctrl_button.h"


namespace App {
	bool SetKeyboard::IsRootPage() const {
		return false;
	}

	SetKeyboard::SetKeyboard():
		App::SettingsPage{ IDD_SET_KEYBOARD },
		m_currentIndex{ 0 }
	{}

	void SetKeyboard::AddShortcut() {
		auto row = m_assigned->AddItem(L"", m_currentIndex);
		m_assigned->ItemColumn(row, 1, L"");
		m_assigned->ItemColumn(row, 2, Intl::GetWString(SIDUndefined));
		m_shortcuts[m_currentIndex] = { L'', KeyAction::Undefined };

		m_currentIndex++;
	}

	void SetKeyboard::SetShortcutFunction() {
		auto newFunction = m_functions->GetSelectionData();
		auto row = m_assigned->GetSelectedRow();
		auto shortcutIndex = m_assigned->GetItemParam(row);
		auto action = static_cast<App::KeyAction>(newFunction);

		m_assigned->ItemColumn(row, 2, Intl::GetWString(App::KeyActionSid(action)));
		m_shortcuts[shortcutIndex].Action = action;
	}

	bool SetKeyboard::PerformOnInitPage() {
		Caption(SIDKeyboard);
		ControlText(IDC_GROUP_KEYBOARD_ACTION, SIDGroupAction);
		ControlText(IDC_GROUP_KEYBOARD_ASSIGNED, SIDGroupAssignedShortcuts);

		ControlText(IDC_TEXT_KEYBOARD_KEY, SIDLabelKey);
		ControlText(IDC_BUTTON_KEYBOARD_ADD, SIDAdd);

		m_functions = CreateComboBox(IDC_COMBO_KEYBOARD_FUNCTION);
		m_functions->Reset();
		m_functions->AddItem(SIDActionNextImage, static_cast<DWORD>(KeyAction::NextImage));
		m_functions->AddItem(SIDActionPrevImage, static_cast<DWORD>(KeyAction::PreviousImage));
		m_functions->AddItem(SIDRenameFilename, static_cast<DWORD>(KeyAction::RenameFile));
		m_functions->OnSelectionChanged.connect([this]() { SetShortcutFunction();  });

		m_assigned = CreateListView(IDC_LIST_KEYBOARD_ASSIGNED);
		m_assigned->AddColumn(L"Key", 50, 0);
		m_assigned->AddColumn(L"Modifiers", 70, 1);
		m_assigned->AddColumn(L"Action", 200, 2);
		m_assigned->Style(LVS_EX_FULLROWSELECT |LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);

		CreateButton(IDC_BUTTON_KEYBOARD_ADD)->OnClick.connect([this]() { AddShortcut(); });

		m_keypress = Keypress::CreateKeypress(IDC_EDIT_KEYBOARD_KEY, Handle());

		SetWindowTheme(m_functions->Handle(), L"Explorer", 0);

		return true;
	}

	void SetKeyboard::onWriteSettings() {
		//throw std::logic_error("The method or operation is not implemented.");
	}
}