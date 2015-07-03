#include "StdAfx.h"
#include "settings_page.h"
#include "settings.h"

namespace App {
	bool SettingsPage::PerformOnInitDialog() {
		auto* pSet = dynamic_cast<App::Settings*>(Parent());
		if (pSet == nullptr) {
			DO_THROW(Err::CriticalError, TX("Settings page had an invalid parent."));
		}

		Move(pSet->GetPagePosition());
		if (!PerformOnInitPage()) return false;
		return true;
	}
}
