#ifndef DLG_VIEW_H
#define DLG_VIEW_H

#include "settings_page.h"
#include "ctrl_combobox.h"

namespace App {
	class SetView:public App::SettingsPage {
	public:
		SetView();

	private:
		void ToggleResizeWindow();

		bool PerformOnInitPage(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		void DoToggleResizeWindow(bool newState);
		void SetupFilterBox(Win::ComboBox* ctrl);

		Win::ComboBox* m_cbDefaultZoom;
		Win::ComboBox* m_cbResizeBehavior;
		Win::ComboBox* m_cbMinFilter;
		Win::ComboBox* m_cbMagFilter;
	};
}

#endif
