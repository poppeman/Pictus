#ifndef PICTUS_DLG_KEYBOARD_H
#define PICTUS_DLG_KEYBOARD_H

#include "settings_page.h"
#include "ctrl_combobox.h"
#include "ctrl_listview.h"
#include "ctrl_keypress.h"
#include "appreg.h"

namespace App {
	class SetKeyboard :public App::SettingsPage {
	public:
		bool IsRootPage() const override;

		SetKeyboard();

	private:
		int AddShortcut();
		void SetShortcutFunction(KeyAction action, int index);
		void OnSetShortcutCombo(KeyboardPress kp);
		void SetShortcutCombo(KeyboardPress kp, int index);

		bool PerformOnInitPage() override;

		void AddAction(KeyAction action, StringID actionSid);

		void PerformUpdateFromSettings(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		Win::ComboBox* m_functions;
		Win::ListView* m_assigned;
		App::Keypress* m_keypress;

		std::map<LPARAM, Reg::KeyboardBinding> m_shortcuts;
		LPARAM m_currentIndex;
	};
}

#endif
