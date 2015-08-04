#ifndef DLG_CONTROLS_H
#define DLG_CONTROLS_H

#include "settings_page.h"
#include "ctrl_combobox.h"

namespace App {
	class SetControls:public App::SettingsPage {
	public:
		SetControls();

	private:
		bool PerformOnInitPage(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		void initMouseButtonList(Win::ComboBox* ctrl, App::MouseAction action);
		void initMouseDblList(Win::ComboBox* ctrl, App::MouseAction action);
		void initMouseWheelList(Win::ComboBox* ctrl, App::MouseAction action);

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
