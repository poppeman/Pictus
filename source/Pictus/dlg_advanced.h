#ifndef DLG_ADVANCED_H
#define DLG_ADVANCED_H

#include "settings_page.h"

namespace App {
	class SetAdvanced:public App::SettingsPage {
	public:
		SetAdvanced();

	private:
		bool PerformOnInitPage();
		void onWriteSettings();
	};
}

#endif
