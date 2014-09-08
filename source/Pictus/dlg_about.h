#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "settings_page.h"

namespace App {
	class SetAbout:public App::SettingsPage {
	public:
		SetAbout();
		~SetAbout();

	private:
		void onWriteSettings() {}
		bool PerformOnInitPage();

		HFONT m_headerFont;
	};
}

#endif
