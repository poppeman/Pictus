#ifndef PICTUS_DLG_KEYBOARD_H
#define PICTUS_DLG_KEYBOARD_H

#include "settings_page.h"
#include "ctrl_keypress.h"
#include "appreg.h"

#include <map>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/listctrl.h>

namespace App
{
	class SetKeyboard:public App::SettingsPage
	{
	public:
		bool IsRootPage() const override;
		std::string Caption() override;

		SetKeyboard(wxWindow* parent);

	private:
		void AddShortcut(int &row, int &id);
		void SetShortcutFunction(KeyAction action, int id);
		void OnSetShortcutCombo(KeyboardPress kp);
		void SetShortcutCombo(KeyboardPress kp, int id);
		void AddAction(KeyAction action, StringID actionSid);
		void UpdateControlStyles();
		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		wxButton *m_removeAssigned;
		wxChoice *m_functions;
		wxListView *m_assigned;
		App::Keypress *m_keypress;

		std::map<int, Reg::KeyboardBinding> m_shortcuts;
		int m_currentId;
		void ResetAssigned() const;
	};
}

#endif
