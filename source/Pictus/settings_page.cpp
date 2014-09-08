#include "StdAfx.h"
#include "settings_page.h"
#include "settings.h"

namespace App {
	bool SettingsPage::PerformOnInitDialog() {
		App::Settings* pSet = dynamic_cast<App::Settings*>(Parent());
		COND_STRICT(pSet, Err::CriticalError, TX("Settings page had an invalid parent."));

		Move(pSet->GetPagePosition());
		if (!PerformOnInitPage()) return false;
		return true;
	}
}
