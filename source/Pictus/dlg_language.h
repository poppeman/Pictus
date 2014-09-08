#ifndef DLG_LANGUAGE_H
#define DLG_LANGUAGE_H

#include "settings_page.h"
#include "app_types.h"
#include "ctrl_combobox.h"

namespace App {
	class SetLanguage:public App::SettingsPage {
	public:
		SetLanguage();

	private:
		bool PerformOnInitPage();
		void onWriteSettings();

		Win::ComboBox* m_cbLang;
	};
}

#endif
