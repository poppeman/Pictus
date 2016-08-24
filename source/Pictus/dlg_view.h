#ifndef PICTUS_DLG_VIEW_H
#define PICTUS_DLG_VIEW_H

#include "settings_page.h"
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>

namespace App
{
	class SetView : public App::SettingsPage
	{
	public:
		std::string Caption() override;
		SetView(wxWindow* parent);

	private:
		//void ToggleResizeWindow();
		void OnClickAdaptWindowSize(wxCommandEvent& event);

		wxSizer* CreateZoomBox();
		wxSizer* CreateViewerBox();

		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		void DoToggleResizeWindow(bool newState);
		wxChoice* SetupFilterBox(wxWindow* parent);

		wxChoice* m_cbDefaultZoom;
		wxChoice* m_cbResizeBehavior;
		wxChoice* m_cbMinFilter;
		wxChoice* m_cbMagFilter;

		wxCheckBox* m_wrapAround;
		wxCheckBox* m_resetZoom;
		wxCheckBox* m_resetPan;
		wxCheckBox* m_adaptWindowSize;

		wxRadioBox* m_positionMethod;
		wxSizer *CreateResizeAlgoSizer(wxWindow* parent);
		wxSizer *CreateModeSizer(wxWindow* parent);

		DECLARE_EVENT_TABLE()
	};
}

#endif
