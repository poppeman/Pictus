#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_interface.h"
#include "registry.h"

namespace App {
	using namespace Reg::Keys;

	bool SetInterface::PerformOnInitPage() {
		Caption(App::SIDInterface);

		SetCheckBox(IDC_CHECK_INTERFACE_SHOWSTATUSBAR, Reg::Key(DWShowStatusBar) != 0);
		SetCheckBox(IDC_CHECK_INTERFACE_ALWAYSONTOP, Reg::Key(DWAlwaysOnTop) != 0);

		ControlText(IDC_GROUP_INTERFACE_STATUSBAR, SIDGroupStatusBar);
		ControlText(IDC_CHECK_INTERFACE_SHOWSTATUSBAR, SIDShowStatusBar);
		ControlText(IDC_CHECK_INTERFACE_ALWAYSONTOP, SIDAlwaysOnTop);
		return true;
	}

	void SetInterface::onWriteSettings() {
		Reg::Key(DWShowStatusBar, GetCheckBox(IDC_CHECK_INTERFACE_SHOWSTATUSBAR));
		Reg::Key(DWAlwaysOnTop, GetCheckBox(IDC_CHECK_INTERFACE_ALWAYSONTOP));
	}

	SetInterface::SetInterface():App::SettingsPage(IDD_SET_INTERFACE) {}

	bool SetInterface::IsRootPage() const {
		return false;
	}
}
