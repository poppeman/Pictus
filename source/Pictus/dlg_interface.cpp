#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_interface.h"
#include "registry.h"

namespace App {
	bool SetInterface::PerformOnInitPage() {
		Caption(App::SIDInterface);

		ControlText(IDC_GROUP_INTERFACE_STATUSBAR, SIDGroupStatusBar);
		ControlText(IDC_CHECK_INTERFACE_SHOWSTATUSBAR, SIDShowStatusBar);
		ControlText(IDC_CHECK_INTERFACE_ALWAYSONTOP, SIDAlwaysOnTop);
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
		App::SettingsPage(IDD_SET_INTERFACE)
	{}

	bool SetInterface::IsRootPage() const {
		return false;
	}
}
