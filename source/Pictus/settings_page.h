#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "appreg.h"

#include <wx/panel.h>

namespace App
{
	class SettingsPage : public wxPanel
	{
	public:
		virtual std::string Caption()=0;
		virtual bool IsRootPage() const;

		void UpdateFromSettings(const Reg::Settings &settings);
		void WriteSettings(Reg::Settings &settings);

		SettingsPage(wxWindow* parent);
		virtual ~SettingsPage();

	private:
		//bool PerformOnInitDialog() override;
		virtual void PerformUpdateFromSettings(const Reg::Settings &settings)=0;
		//virtual bool PerformOnInitPage() = 0;
		virtual void onWriteSettings(Reg::Settings &settings) = 0;
	};
}

#endif
