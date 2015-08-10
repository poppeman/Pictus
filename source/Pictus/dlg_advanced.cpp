#include "StdAfx.h"
#include "dlg_advanced.h"
#include "registry.h"
#include "res_settings.h"

namespace App {
	using namespace Intl;

	bool SetAdvanced::PerformOnInitPage() {
		Caption(SIDAdvanced);
		ControlText(IDC_MULT_INSTANCE, SIDMultiInstance);
		return true;
	}

	void SetAdvanced::PerformUpdateFromSettings(const Reg::Settings& settings) {
		SetCheckBox(IDC_MULT_INSTANCE, settings.View.MultipleInstances);
	}

	void SetAdvanced::onWriteSettings(Reg::Settings& settings) {
		settings.View.MultipleInstances = GetCheckBox(IDC_MULT_INSTANCE);
	}

	SetAdvanced::SetAdvanced():App::SettingsPage(IDD_SET_ADVANCED) {}
}
