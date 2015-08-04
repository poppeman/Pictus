#ifndef DLG_INTERFACE_H
#define DLG_INTERFACE_H

#include "settings_page.h"

namespace App {
	class SetInterface:public App::SettingsPage {
	public:
		bool IsRootPage() const override;

		SetInterface();

	private:
		bool PerformOnInitPage(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;
	};
}

#endif
