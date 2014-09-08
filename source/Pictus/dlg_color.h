#ifndef DLG_COLOR_H
#define DLG_COLOR_H

#include "settings_page.h"

namespace App {
	class SetColor:public App::SettingsPage {
	public:
		bool IsRootPage();

		SetColor();

	private:
		void RegisterWindowClasses();
		void PerformOnCreate();
		bool PerformOnCommand(int id, WPARAM wp, LPARAM lp);
		bool PerformOnVScroll(WPARAM wp, LPARAM lp);
		bool PerformOnApp(int index, WPARAM wParam, LPARAM lParam);

		bool PerformOnInitPage();
		void onWriteSettings();
		void updatePreviewColor();

	private:
		DWORD m_currCol;
		bool m_isUpdating;
	};
}

#endif
