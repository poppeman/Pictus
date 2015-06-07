#ifndef PICTUS_DLG_KEYBOARD_H
#define PICTUS_DLG_KEYBOARD_H

#include "settings_page.h"
#include "ctrl_combobox.h"
#include "ctrl_listview.h"

namespace App {
	class SetKeyboard :public App::SettingsPage {
	public:
		bool IsRootPage() const override;

		SetKeyboard();

	private:
		void AddShortcut();

		bool PerformOnInitPage() override;
		void onWriteSettings() override;

		Win::ComboBox* m_functions;
		Win::ListView* m_assigned;
	};
}

#endif
