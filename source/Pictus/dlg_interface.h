#ifndef DLG_INTERFACE_H
#define DLG_INTERFACE_H

#include <wx/checkbox.h>
#include "settings_page.h"

namespace App
{
	class SetInterface : public App::SettingsPage
	{
	public:
		bool IsRootPage() const override;
		std::string Caption() override;

		SetInterface(wxWindow *parent);

	private:
		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		wxCheckBox *m_showStatusBar;
		wxCheckBox *m_alwaysOnTop;
	};
}

#endif
