#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "dialog.h"
#include "appreg.h"

namespace App {
	class SettingsPage:public Win::Dialog {
	public:
		void getSettings(const Reg::Settings& settings) { PerformOnInitPage(settings); }
		virtual bool IsRootPage() const { return true; }

		void WriteSettings(Reg::Settings& settings) { onWriteSettings(settings); }

		SettingsPage(int id) :Win::Dialog(id) {}
		virtual ~SettingsPage() {}

	private:
		bool PerformOnInitDialog(const Reg::Settings& settings);
		virtual bool PerformOnInitPage(const Reg::Settings& settings) = 0;
		virtual void onWriteSettings(Reg::Settings& settings) = 0;
	};
}

#endif
