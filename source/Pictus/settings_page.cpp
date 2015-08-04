#include "StdAfx.h"
#include "settings_page.h"
#include "settings.h"

namespace App {
	bool SettingsPage::PerformOnInitDialog(const Reg::Settings& settings) {
		auto* pSet = dynamic_cast<App::Settings*>(Parent());
		if (pSet == nullptr) {
			DO_THROW(Err::CriticalError, L"Settings page had an invalid parent.");
		}

		Move(pSet->GetPagePosition());
		return PerformOnInitPage(settings);
	}
}
