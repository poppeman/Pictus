#ifndef DLG_INTERFACE_H
#define DLG_INTERFACE_H

#include "settings_page.h"

namespace App {
	class SetInterface:public App::SettingsPage {
	public:
		bool IsRootPage();

		SetInterface();

	private:
		bool PerformOnInitPage();
		void onWriteSettings();
	};
}

#endif
