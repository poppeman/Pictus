#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "settings_page.h"

namespace App {
	class SetAbout:public App::SettingsPage {
	public:
		SetAbout();
		~SetAbout();

	private:
		bool PerformOnInitPage(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		HFONT m_headerFont;
	};
}

#endif
