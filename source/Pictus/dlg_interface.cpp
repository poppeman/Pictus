#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_interface.h"
#include "registry.h"

namespace App {
	bool SetInterface::PerformOnInitPage() {
		Caption(App::SIDSettingsInterface);

		ControlText(IDC_GROUP_INTERFACE_STATUSBAR, SIDSettingsInterfaceStatusBar);
		ControlText(IDC_CHECK_INTERFACE_SHOWSTATUSBAR, SIDSettingsInterfaceShowStatusBar);
		ControlText(IDC_CHECK_INTERFACE_ALWAYSONTOP, SIDSettingsInterfaceAlwaysOnTop);
		return true;
	}

	void SetInterface::PerformUpdateFromSettings(const Reg::Settings& settings) {
		SetCheckBox(IDC_CHECK_INTERFACE_SHOWSTATUSBAR, settings.View.ShowStatusBar);
		SetCheckBox(IDC_CHECK_INTERFACE_ALWAYSONTOP, settings.View.AlwaysOnTop);
	}

	void SetInterface::onWriteSettings(Reg::Settings& settings) {
		settings.View.ShowStatusBar = GetCheckBox(IDC_CHECK_INTERFACE_SHOWSTATUSBAR);
		settings.View.AlwaysOnTop = GetCheckBox(IDC_CHECK_INTERFACE_ALWAYSONTOP);
	}

	SetInterface::SetInterface():
		App::SettingsPage{ IDD_SET_INTERFACE }
	{}

	bool SetInterface::IsRootPage() const {
		return false;
	}
}
