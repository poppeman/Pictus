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
		m_currentId{ 0 },
		m_functions{ nullptr },
		m_assigned{ nullptr },
		m_keypress{ nullptr }
	{}

	void SetKeyboard::AddShortcut(int& row, LPARAM& id) {
		row = m_assigned->AddItem(Intl::GetWString(SIDSettingsKeyboardNotSet), m_currentId);
		m_assigned->ItemColumn(row, 1, L"");
		m_assigned->ItemColumn(row, 2, Intl::GetWString(SIDSettingsKeyboardNotSet));
		m_shortcuts[m_currentId] = { { L'', false, false, false }, KeyAction::Undefined };
		id = m_currentId++;
	}

	void SetKeyboard::SetShortcutFunction(App::KeyAction action, LPARAM id) {
		for (auto row = 0; row < m_assigned->Size(); row++) {
			if (m_assigned->GetItemParam(row) == id) {
				m_assigned->ItemColumn(row, 2, Intl::GetWString(App::KeyActionSid(action)));
				m_shortcuts[id].Action = action;
			}
		}
	}

	void SetKeyboard::OnSetShortcutCombo(KeyboardPress kp) {
		auto row = m_assigned->GetSelectedRow();
		auto shortcutIndex = m_assigned->GetItemParam(row);

		SetShortcutCombo(kp, shortcutIndex);
	}


	void SetKeyboard::SetShortcutCombo(KeyboardPress kp, LPARAM index) {
		for (auto row = 0; row < m_assigned->Size(); row++) {
			if (m_assigned->GetItemParam(row) == index) {
				auto tmp = GetKeyString(kp.Key);

				m_assigned->ItemColumn(row, 0, tmp);

				std::vector<std::wstring> mods;
				if (kp.Alt) mods.push_back({ L"Alt" });
				if (kp.Shift) mods.push_back({ L"Shift" });
				if (kp.Ctrl) mods.push_back({ L"Ctrl" });
				m_assigned->ItemColumn(row, 1, Implode(mods, L"+"));

				m_shortcuts[index].Key = kp;
			}
		}
	}


	void SetKeyboard::AddAction(KeyAction action, StringID actionSid) {
		m_functions->AddItem(actionSid, static_cast<DWORD>(action));
	}


	bool SetKeyboard::PerformOnInitPage() {
		Caption(SIDSettingsKeyboard);
		ControlText(IDC_GROUP_KEYBOARD_ACTION, SIDSettingsKeyboardAction);
		ControlText(IDC_GROUP_KEYBOARD_ASSIGNED, SIDSettingsKeyboardAssigned);

		ControlText(IDC_TEXT_KEYBOARD_KEY, SIDSettingsKeyboardKeys);
		ControlText(IDC_BUTTON_KEYBOARD_ADD, SIDSettingsKeyboardAdd);

		m_functions = CreateComboBox(IDC_COMBO_KEYBOARD_FUNCTION);
		m_functions->Reset();

		for (const auto& kas : App::ActionSids) {
			AddAction(kas.first, kas.second);
		}
		m_functions->OnSelectionChanged.connect([this]() { 
			auto row = m_assigned->GetSelectedRow();
			auto index = m_assigned->GetItemParam(row);

			auto newFunction = m_functions->GetSelectionData();
			auto action = static_cast<App::KeyAction>(newFunction);
			SetShortcutFunction(action, index);
		});

		m_assigned = CreateListView(IDC_LIST_KEYBOARD_ASSIGNED);
		m_assigned->AddColumn(L"Key", 75, 0);
		m_assigned->AddColumn(L"Modifiers", 85, 1);
		m_assigned->AddColumn(L"Action", 150, 2);
		m_assigned->Style(LVS_EX_FULLROWSELECT |LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP | LVS_EX_ONECLICKACTIVATE);
		m_assigned->OnSelectionChanged = [&](int row) {
			if (row != -1) {
				auto index = m_assigned->GetItemParam(row);
				if (index != -1) {
					m_functions->SetSelection(static_cast<DWORD>(m_shortcuts[index].Action));
					m_keypress->SetCombo(m_shortcuts[index].Key);
				}
			}
			UpdateControlStyles();
		};

		CreateButton(IDC_BUTTON_KEYBOARD_ADD)->OnClick.connect([this]() { 
			int row;
			LPARAM id;
			AddShortcut(row, id);
			m_assigned->SetSelectedRow(row);
		});

		CreateButton(IDC_BUTTON_KEYBOARD_REMOVE)->OnClick.connect([this]() {
			auto row = m_assigned->GetSelectedRow();
			if (row != -1) {
				auto index = m_assigned->GetItemParam(row);
				m_shortcuts.erase(index);
				m_assigned->RemoveItem(row);
			}
			UpdateControlStyles();
		});

		m_keypress = Keypress::CreateKeypress(IDC_EDIT_KEYBOARD_KEY, Handle());
		m_keypress->OnNewCombo = [=](App::KeyboardPress kp) {
			OnSetShortcutCombo(kp);
		};

		UpdateControlStyles();

		SetWindowTheme(m_functions->Handle(), L"Explorer", 0);

		return true;
	}

	void SetKeyboard::PerformUpdateFromSettings(const Reg::Settings& settings) {
		for (auto binding : settings.Keyboard.Bindings) {
			int row;
			LPARAM id;
			AddShortcut(row, id);
			SetShortcutCombo(binding.Key, id);
			SetShortcutFunction(binding.Action, id);
		}
	}

	void SetKeyboard::onWriteSettings(Reg::Settings& settings) {
		settings.Keyboard.Bindings.clear();

		for (const auto& binding : m_shortcuts) {
			if (binding.second.Action != KeyAction::Undefined) {
				settings.Keyboard.Bindings.push_back(binding.second);
			}
		}
		//throw std::logic_error("The method or operation is not implemented.");
	}

	void SetKeyboard::UpdateControlStyles() {
		bool enable = (m_assigned->GetSelectedRow() != -1);
		CreateButton(IDC_BUTTON_KEYBOARD_REMOVE)->Enabled(enable);
		m_functions->Enabled(enable);
		m_keypress->Enabled(enable);
	}

}
