#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "dialog.h"

namespace App {
	class SettingsPage:public Win::Dialog {
	public:
		void getSettings() { PerformOnInitPage(); }
		virtual bool IsRootPage() { return true; }

		void WriteSettings() { onWriteSettings(); }

		SettingsPage(int id):Win::Dialog(id) {}
		virtual ~SettingsPage() {}

	private:
		bool PerformOnInitDialog();
		virtual bool PerformOnInitPage()=0;
		virtual void onWriteSettings()=0;
	};
}

#endif
