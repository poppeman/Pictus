#ifndef DLG_LANGUAGE_H
#define DLG_LANGUAGE_H

#include "settings_page.h"
#include "app_types.h"

#include <wx/choice.h>

namespace App
{
	class SetLanguage:public SettingsPage
	{
	public:
		std::string Caption() override;
		SetLanguage(wxWindow* parent);

	private:
		void PerformUpdateFromSettings(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		void SetSelection(Intl::Language lang);
		Intl::Language GetSelection();

		wxChoice* m_cbLang;
	};
}

#endif
