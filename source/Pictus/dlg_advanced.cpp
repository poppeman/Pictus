#include "StdAfx.h"
#include "dlg_advanced.h"
#include "registry.h"
#include "res_settings.h"

namespace App {
	using namespace Reg::Keys;
	using namespace Intl;

	bool SetAdvanced::PerformOnInitPage() {
		Caption(SIDAdvanced);
		ControlText(IDC_MULT_INSTANCE, SIDMultiInstance);

		SetCheckBox(IDC_MULT_INSTANCE, Reg::Key(DWMultipleInstances) != 0);
		return true;
	}

	void SetAdvanced::onWriteSettings() {
		Reg::Key(DWMultipleInstances, GetCheckBox(IDC_MULT_INSTANCE));
	}

	SetAdvanced::SetAdvanced():App::SettingsPage(IDD_SET_ADVANCED) {}
}
