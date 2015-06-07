#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_keyboard.h"

#include "ctrl_button.h"


namespace App {
	bool SetKeyboard::IsRootPage() const {
		return false;
	}

	SetKeyboard::SetKeyboard():App::SettingsPage(IDD_SET_KEYBOARD) {}

	void SetKeyboard::AddShortcut() {
		auto row = m_assigned->AddItem(L"O");
		m_assigned->ItemColumn(row, 1, L"DEHUMANIZE YOURSELF AND FACE TO BLOODSHED");

		m_assigned->ItemColumn(row, 2, Intl::GetWString(m_functions->GetSelectionData()));
	}

	bool SetKeyboard::PerformOnInitPage() {
		Caption(SIDKeyboard);
		ControlText(IDC_GROUP_KEYBOARD_ACTION, SIDGroupAction);
		ControlText(IDC_GROUP_KEYBOARD_ASSIGNED, SIDGroupAssignedShortcuts);

		ControlText(IDC_TEXT_KEYBOARD_KEY, SIDLabelKey);
		ControlText(IDC_BUTTON_KEYBOARD_ADD, SIDAssign);

		m_functions = CreateComboBox(IDC_COMBO_KEYBOARD_FUNCTION);
		m_functions->Reset();
		m_functions->AddItem(SIDRenameWindow, SIDRenameWindow);
		m_functions->AddItem(SIDRandom, SIDRandom);

		m_assigned = CreateListView(IDC_LIST_KEYBOARD_ASSIGNED);
		m_assigned->AddColumn(L"Key", 50, 0);
		m_assigned->AddColumn(L"Modifiers", 70, 1);
		m_assigned->AddColumn(L"Action", 200, 2);

		CreateButton(IDC_BUTTON_KEYBOARD_ADD)->OnClick.connect([this]() { AddShortcut(); });

		//throw std::logic_error("The method or operation is not implemented.");
		return true;
	}

	void SetKeyboard::onWriteSettings() {
		//throw std::logic_error("The method or operation is not implemented.");
	}
}
