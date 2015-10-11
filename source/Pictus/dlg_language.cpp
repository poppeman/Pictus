#include "res_settings.h"
#include "dlg_language.h"
#include "registry.h"

namespace App {
	using namespace Intl;

	bool SetLanguage::PerformOnInitPage() {
		Caption(App::SIDSettingsLanguage);
		ControlText(IDC_GROUP_LANG, SIDSettingsLanguage);

		m_cbLang = CreateComboBox(IDC_COMBO_LANG);
		for (auto i = 0; i < static_cast<int>(Intl::Language::Undefined); ++i) {
			m_cbLang->AddItem(UTF8ToWString(Intl::GetStringLang(SIDCurrentLanguage, static_cast<Intl::Language>(i))), i);
		}

		m_cbLang->SetSelection(static_cast<int>(Intl::CurrentLanguage()));
		return true;
	}

	void SetLanguage::PerformUpdateFromSettings(const Reg::Settings& settings) {

	}


	void SetLanguage::onWriteSettings(Reg::Settings& settings) {
		settings.View.Language = Intl::Language(m_cbLang->GetSelectionData());
		CurrentLanguage(Language(m_cbLang->GetSelectionData()));
	}

	SetLanguage::SetLanguage():
		App::SettingsPage{ IDD_SET_LANGUAGE }
	{}
}
