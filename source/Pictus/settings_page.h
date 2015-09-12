#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "dialog.h"
#include "appreg.h"

namespace App {
	class SettingsPage:public Win::Dialog {
	public:
		virtual bool IsRootPage() const;

		void UpdateFromSettings(const Reg::Settings& settings);
		void WriteSettings(Reg::Settings& settings);

		SettingsPage(int id);
		virtual ~SettingsPage();

	private:
		bool PerformOnInitDialog() override;
		virtual void PerformUpdateFromSettings(const Reg::Settings& settings)=0;
		virtual bool PerformOnInitPage() = 0;
		virtual void onWriteSettings(Reg::Settings& settings) = 0;
	};
}

#endif
