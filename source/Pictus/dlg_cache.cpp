#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_cache.h"
#include "registry.h"
#include "ctrl_button.h"

namespace App {
	using namespace Intl;
	using namespace Win;

	bool SetPageCache::PerformOnInitPage(const Reg::Settings& settings) {
		CreateButton(IDC_AUTOMEM)->OnClick.connect([this]() { UpdateControls(); });

		Caption(SIDCache);
		ControlText(IDC_GROUP_MEMORY, SIDGroupCache);
		ControlText(IDC_AUTOMEM, SIDAutoMemLimit);
		ControlText(IDC_MEM_MB, SIDMBytes);

		m_cacheSize = CreateEditBox(IDC_MEMLIMIT);
		m_cacheSize->Filterchars(EditBox::FilterNotNumerical, SIDNumericalInvalid);
		m_cacheSize->Text(ToWString(settings.Cache.ManualMemoryLimit));

		SetCheckBox(IDC_AUTOMEM, settings.Cache.DoAutoMemoryLimit);
		UpdateControls();

		return true;
	}

	void SetPageCache::onWriteSettings(Reg::Settings& settings) {
		settings.Cache.DoAutoMemoryLimit = GetCheckBox(IDC_AUTOMEM) != 0;
		settings.Cache.ManualMemoryLimit = FromWString<int>(m_cacheSize->Text());
	}

	void SetPageCache::UpdateControls() {
		EnableWindow(GetDlgItem(Handle(), IDC_AUTOMEM), true);

		if (!IsDlgButtonChecked(Handle(), IDC_AUTOMEM)) {
			EnableWindow(GetDlgItem(Handle(), IDC_MEMLIMIT), true);
		}
		else {
			EnableWindow(GetDlgItem(Handle(), IDC_MEMLIMIT), false);
		}
	}

	SetPageCache::SetPageCache():
		App::SettingsPage(IDD_SET_MEMORY)
	{}
}
