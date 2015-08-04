#include "StdAfx.h"
#include "dlg_advanced.h"
#include "registry.h"
#include "res_settings.h"

namespace App {
	using namespace Intl;

	bool SetAdvanced::PerformOnInitPage(const Reg::Settings& settings) {
		Caption(SIDAdvanced);
		ControlText(IDC_MULT_INSTANCE, SIDMultiInstance);

		SetCheckBox(IDC_MULT_INSTANCE, settings.View.MultipleInstances);
		return true;
	}

	void SetAdvanced::onWriteSettings(Reg::Settings& settings) {
		settings.View.MultipleInstances = GetCheckBox(IDC_MULT_INSTANCE);
	}

	SetAdvanced::SetAdvanced():App::SettingsPage(IDD_SET_ADVANCED) {}
}
