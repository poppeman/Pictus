#ifndef PICTUS_DLG_KEYBOARD_H
#define PICTUS_DLG_KEYBOARD_H

#include "settings_page.h"
#include "ctrl_combobox.h"
#include "ctrl_listview.h"
#include "ctrl_keypress.h"

namespace App {
	class SetKeyboard :public App::SettingsPage {
	public:
		bool IsRootPage() const override;

		SetKeyboard();

	private:
		void AddShortcut();
		void SetShortcutFunction();

		bool PerformOnInitPage() override;
		void PerformUpdateFromSettings(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		Win::ComboBox* m_functions;
		Win::ListView* m_assigned;
		App::Keypress* m_keypress;

		struct ShortcutEntry {
			wchar_t Key;
			KeyAction Action;
		};

		std::map<LPARAM, ShortcutEntry> m_shortcuts;
		LPARAM m_currentIndex;
	};
}

#endif
