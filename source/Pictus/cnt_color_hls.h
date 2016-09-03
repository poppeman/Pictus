#ifndef PICTUS_CNT_COLOR_HLS_H
#define PICTUS_CNT_COLOR_HLS_H

#include <wx/panel.h>
#include <illa/color.h>
#include <orz/geom.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>

wxDECLARE_EVENT(COLOR_CHANGED_HLS, wxCommandEvent);

namespace App
{
	class ControlColorHls:public wxStaticBox
	{
	public:
		ControlColorHls(wxWindow *parent, wxWindowID winid);

		void SetHls(Img::HLSTriplet col);
		Img::HLSTriplet GetHls();

	private:
		wxSpinCtrl *m_h, *m_l, *m_s;

		void OnChange(wxSpinEvent& evt);

		enum {
			HId,
			LId,
			SId
		};

		DECLARE_EVENT_TABLE()
	};
}

#endif
