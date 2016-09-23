#include "dlg_keyboard.h"

#include "app_types.h"
#include "orz/types.h"
#include "wintypes.h"

#include "settings_layout.h"

#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/button.h>

namespace App
{
	class ActionClientData:public wxClientData
	{
	public:
		KeyAction Action;
		ActionClientData(App::KeyAction action):Action(action) {}
	};

	bool SetKeyboard::IsRootPage() const
	{
		return false;
	}

	std::string SetKeyboard::Caption()
	{
		return Intl::GetString(SIDSettingsKeyboard);
	}

	SetKeyboard::SetKeyboard(wxWindow* parent):
		App::SettingsPage{parent},
		m_functions{nullptr},
		m_assigned{nullptr},
		m_keypress{nullptr},
		m_currentId{0}
	{
		auto assignedBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsKeyboardAssigned));
		m_assigned = new wxListView(assignedBox->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize);
		assignedBox->Add(m_assigned, StaticBoxInnerPadding(0));
		ResetAssigned();
		//m_assigned->Style(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP | LVS_EX_ONECLICKACTIVATE);
		m_assigned->Bind(wxEVT_LIST_ITEM_SELECTED, [&](wxListEvent& evt)
		{
			auto row = evt.GetIndex();
			if (row != -1)
			{
				auto index = m_assigned->GetItemData(row);
				//if (index != -1)
				{
					for(unsigned i=0;i<m_functions->GetCount();i++)
					{
						if(dynamic_cast<ActionClientData*>(m_functions->GetClientObject(i))->Action == m_shortcuts[index].Action)
						{
							m_functions->SetSelection(i);
							break;
						}
					}
					m_keypress->SetCombo(m_shortcuts[index].Key);
				}
			}
			UpdateControlStyles();
		});

		auto addButton = new wxButton(assignedBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsKeyboardAdd));
		addButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt)
			{
				int row;
				int id;
				AddShortcut(row, id);
				m_assigned->Select(row);
			}
		);
		m_removeAssigned = new wxButton(assignedBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsKeyboardRemove));

		m_removeAssigned->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt)
			{
				auto row = m_assigned->GetFirstSelected();
				if (row != -1)
				{
					auto index = m_assigned->GetItemData(row);
					m_shortcuts.erase(index);
					m_assigned->DeleteItem(row);
				}
				UpdateControlStyles();
			}
		);

		auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonSizer->Add(addButton, wxSizerFlags(0));
		buttonSizer->Add(m_removeAssigned, ButtonPadding());

		assignedBox->Add(buttonSizer, ButtonToolbarPadding());

		auto actionBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsKeyboardAction));
		m_functions = new wxChoice(actionBox->GetStaticBox(), wxID_ANY);
		m_keypress = new Keypress(actionBox->GetStaticBox());
		actionBox->Add(m_functions, StaticBoxInnerPadding(0));
		actionBox->Add(new wxStaticText(actionBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsKeyboardKeys)), StaticBoxInnerPadding(0));
		actionBox->Add(m_keypress, StaticBoxInnerPadding(0));

		for (const auto &kas : App::ActionSids)
		{
			AddAction(kas.first, kas.second);
		}
		m_functions->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
			auto row = m_assigned->GetFirstSelected();
			auto index = m_assigned->GetItemData(row);

			auto action= dynamic_cast<ActionClientData*>(m_functions->GetClientObject(m_functions->GetSelection()))->Action;
			SetShortcutFunction(action, index);
		});
		m_keypress->OnNewCombo = [=](App::KeyboardPress kp)
		{
			OnSetShortcutCombo(kp);
		};



		auto topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(assignedBox, wxSizerFlags(0).Expand());
		topSizer->Add(actionBox, wxSizerFlags(0).Expand());

		SetSizerAndFit(topSizer);
		UpdateControlStyles();

		/*UpdateControlStyles();

		SetWindowTheme(m_functions->Handle(), L"Explorer", 0);

		return true;*/
	}

	void SetKeyboard::ResetAssigned() const
	{
		m_assigned->AppendColumn(Win::GetStringWx(SIDSettingsKeyboardKey), wxLIST_FORMAT_LEFT);
		m_assigned->AppendColumn(Win::GetStringWx(SIDSettingsKeyboardModifiers), wxLIST_FORMAT_LEFT);
		m_assigned->AppendColumn(Win::GetStringWx(SIDSettingsKeyboardAction), wxLIST_FORMAT_LEFT);
	}

	void SetKeyboard::AddShortcut(int &row, int &id)
	{
		row = m_assigned->InsertItem(0, Win::GetStringWx(SIDSettingsKeyboardNotSet));
		m_assigned->SetItem(row, 1, L"");
		m_assigned->SetItem(row, 2, UTF8ToWString(Intl::GetString(SIDSettingsKeyboardNotSet)));
		m_assigned->SetItemData(row, m_currentId);

		m_shortcuts[m_currentId] = {{0, false, false, false}, KeyAction::Undefined};
		id = m_currentId++;
	}

	void SetKeyboard::SetShortcutFunction(App::KeyAction action, int id)
	{
		for (auto row = 0; row < m_assigned->GetItemCount(); row++)
		{
			if (m_assigned->GetItemData(row) == (unsigned)id)
			{
				m_assigned->SetItem(row, 2, UTF8ToWString(Intl::GetString(App::KeyActionSid(action))));
				m_shortcuts[id].Action = action;
			}
		}
	}

	void SetKeyboard::OnSetShortcutCombo(KeyboardPress kp)
	{
		auto row = m_assigned->GetFirstSelected();
		auto shortcutIndex = m_assigned->GetItemData(row);

		SetShortcutCombo(kp, shortcutIndex);
	}


	void SetKeyboard::SetShortcutCombo(KeyboardPress kp, int index)
	{
		for (auto row = 0; row < m_assigned->GetItemCount(); row++)
		{
			if (m_assigned->GetItemData(row) == (unsigned)index)
			{
				auto tmp = GetKeyString((wxKeyCode) kp.Key);

				m_assigned->SetItem(row, 0, tmp);

				std::vector<std::wstring> mods;
				if (kp.Alt) mods.push_back({L"Alt"});
				if (kp.Shift) mods.push_back({L"Shift"});
				if (kp.Ctrl) mods.push_back({L"Ctrl"});
				m_assigned->SetItem(row, 1, Implode(mods, L"+"));

				m_shortcuts[index].Key = kp;
			}
		}
	}


	void SetKeyboard::AddAction(KeyAction action, StringID actionSid)
	{
		m_functions->Append(Win::GetStringWx(actionSid), new ActionClientData(action));
	}

	void SetKeyboard::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_assigned->ClearAll();
		ResetAssigned();
		m_shortcuts.clear();

		for (auto binding : settings.Keyboard.Bindings)
		{
			int row;
			int id;
			AddShortcut(row, id);
			SetShortcutCombo(binding.Key, id);
			SetShortcutFunction(binding.Action, id);
		}
	}

	void SetKeyboard::onWriteSettings(Reg::Settings &settings)
	{
		settings.Keyboard.Bindings.clear();

		for (const auto &binding : m_shortcuts)
		{
			if (binding.second.Action != KeyAction::Undefined)
			{
				settings.Keyboard.Bindings.push_back(binding.second);
			}
		}
	}

	void SetKeyboard::UpdateControlStyles()
	{
		bool enable = (m_assigned->GetFirstSelected() != -1);
		m_removeAssigned->Enable(enable);
		m_functions->Enable(enable);
		m_keypress->Enable(enable);
	}
}
