#ifndef DLG_ADVANCED_H
#define DLG_ADVANCED_H

#include "settings_page.h"
#include <wx/checkbox.h>

namespace App
{
	class SetAdvanced : public App::SettingsPage
	{
	public:
		std::string Caption() override;
		SetAdvanced(wxWindow* parent);

	private:
		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		wxCheckBox* m_allowMultipleInstances;
	};
}

#endif
