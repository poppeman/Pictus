#ifndef DLG_CONTROLS_H
#define DLG_CONTROLS_H

#include "settings_page.h"
#include <wx/choice.h>
#include <wx/sizer.h>

namespace App
{
	class SetControls : public App::SettingsPage
	{
	public:
		std::string Caption() override;
		SetControls(wxWindow* parent);

	private:
		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		App::MouseAction GetComboAction(wxChoice* cb);
		void SetComboAction(wxChoice* cb, App::MouseAction action);

		wxChoice* initMouseButtonList(wxStaticBoxSizer* parent, StringID label);
		wxChoice* initMouseDblList(wxStaticBoxSizer* parent, StringID label);
		wxChoice* initMouseWheelList(wxStaticBoxSizer* parent, StringID label);

		wxChoice* m_cbLeftMouse;
		wxChoice* m_cbMiddleMouse;
		wxChoice* m_cbRightMouse;
		wxChoice* m_cbExtra1Mouse;
		wxChoice* m_cbExtra2Mouse;
		wxChoice* m_cbLeftMouseDoubleClick;
		wxChoice* m_cbMiddleMouseDoubleClick;
		wxChoice* m_cbRightMouseDoubleClick;
		wxChoice* m_cbExtra1MouseDoubleClick;
		wxChoice* m_cbExtra2MouseDoubleClick;
		wxChoice* m_cbWheelUp;
		wxChoice* m_cbWheelDown;
	};
}

#endif
