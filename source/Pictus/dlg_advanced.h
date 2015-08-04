#ifndef DLG_ADVANCED_H
#define DLG_ADVANCED_H

#include "settings_page.h"

namespace App {
	class SetAdvanced:public App::SettingsPage {
	public:
		SetAdvanced();

	private:
		bool PerformOnInitPage(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;
	};
}

#endif
