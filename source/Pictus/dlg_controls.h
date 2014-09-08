#ifndef DLG_CONTROLS_H
#define DLG_CONTROLS_H

#include "settings_page.h"
#include "ctrl_combobox.h"

namespace App {
	class SetControls:public App::SettingsPage {
	public:
		SetControls();

	private:
		bool PerformOnInitPage();

		void onWriteSettings();
		void initMouseButtonList(Win::ComboBox* ctrl, DWORD key, bool resetSelection=true);
		void initMouseDblList(Win::ComboBox* ctrl, DWORD key, bool resetSelection=true);
		void initMouseWheelList(Win::ComboBox* ctrl, DWORD key, bool resetSelection=true);

		Win::ComboBox* m_cbLeftMouse;
		Win::ComboBox* m_cbMiddleMouse;
		Win::ComboBox* m_cbRightMouse;

		Win::ComboBox* m_cbLeftMouseDoubleClick;
		Win::ComboBox* m_cbMiddleMouseDoubleClick;
		Win::ComboBox* m_cbRightMouseDoubleClick;

		Win::ComboBox* m_cbWheelUp;
		Win::ComboBox* m_cbWheelDown;
	};
}

#endif
