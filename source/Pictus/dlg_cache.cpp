#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_cache.h"
#include "registry.h"
#include "ctrl_button.h"

namespace App {
	using namespace Reg::Keys;
	using namespace Intl;
	using namespace Win;

	bool SetPageCache::PerformOnInitPage() {
		CreateButton(IDC_AUTOMEM)->OnClick.connect([this]() { UpdateControls(); });

		Caption(SIDCache);
		ControlText(IDC_GROUP_MEMORY, SIDGroupCache);
		ControlText(IDC_AUTOMEM, SIDAutoMemLimit);
		ControlText(IDC_MEM_MB, SIDMBytes);

		m_cacheSize = CreateEditBox(IDC_MEMLIMIT);
		m_cacheSize->Filterchars(EditBox::FilterNotNumerical, SIDNumericalInvalid);
		m_cacheSize->Text(ToWString(Reg::Key(DWManualMemoryLimit)));

		SetCheckBox(IDC_AUTOMEM, Reg::Key(DWDoAutoMemoryLimit)!=0);
		UpdateControls();

		return true;
	}

	void SetPageCache::onWriteSettings() {
		Reg::Key(DWDoAutoMemoryLimit, GetCheckBox(IDC_AUTOMEM));
		Reg::Key(DWManualMemoryLimit, FromWString<int>(m_cacheSize->Text()));
	}

	void SetPageCache::UpdateControls() {
		EnableWindow(GetDlgItem(Handle(), IDC_AUTOMEM), true);

		if (!IsDlgButtonChecked(Handle(), IDC_AUTOMEM))
			EnableWindow(GetDlgItem(Handle(), IDC_MEMLIMIT), true);
		else
			EnableWindow(GetDlgItem(Handle(), IDC_MEMLIMIT), false);
	}

	SetPageCache::SetPageCache():App::SettingsPage(IDD_SET_MEMORY) {}
}
