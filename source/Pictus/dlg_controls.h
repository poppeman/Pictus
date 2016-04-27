#ifndef DLG_CONTROLS_H
#define DLG_CONTROLS_H

#include "settings_page.h"
#include "ctrl_combobox.h"

namespace App {
	class SetControls:public App::SettingsPage {
	public:
		SetControls();

	private:
		bool PerformOnInitPage() override;
		void PerformUpdateFromSettings(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		void initMouseButtonList(Win::ComboBox* ctrl);
		void initMouseDblList(Win::ComboBox* ctrl);
		void initMouseWheelList(Win::ComboBox* ctrl);

		Win::ComboBox* m_cbLeftMouse;
		Win::ComboBox* m_cbMiddleMouse;
		Win::ComboBox* m_cbRightMouse;
		Win::ComboBox* m_cbExtra1Mouse;
		Win::ComboBox* m_cbExtra2Mouse;

		Win::ComboBox* m_cbLeftMouseDoubleClick;
		Win::ComboBox* m_cbMiddleMouseDoubleClick;
		Win::ComboBox* m_cbRightMouseDoubleClick;
		Win::ComboBox* m_cbExtra1MouseDoubleClick;
		Win::ComboBox* m_cbExtra2MouseDoubleClick;

		Win::ComboBox* m_cbWheelUp;
		Win::ComboBox* m_cbWheelDown;
	};
}

#endif
