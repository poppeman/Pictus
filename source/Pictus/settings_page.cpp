#include "StdAfx.h"
#include "settings_page.h"
#include "settings.h"

namespace App {
	bool SettingsPage::PerformOnInitDialog() {
		auto* pSet = dynamic_cast<App::Settings*>(Parent());
		if (pSet == nullptr) {
			DO_THROW(Err::CriticalError, L"Settings page had an invalid parent.");
		}

		return PerformOnInitPage();
	}

	void SettingsPage::UpdateFromSettings(const Reg::Settings& settings) {
		if (IsModelessVisible()) {
			PerformUpdateFromSettings(settings);
		}
	}

}
