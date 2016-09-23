#ifndef PICTUS_CNT_COLOR_RGB_H
#define PICTUS_CNT_COLOR_RGB_H

#include <wx/panel.h>
#include <illa/color.h>
#include <orz/geom.h>
#include <wx/spinctrl.h>

wxDECLARE_EVENT(COLOR_CHANGED_RGB, wxCommandEvent);

namespace App
{
	class ControlColorRgb:public wxPanel
	{
	public:
		ControlColorRgb(wxWindow *parent, wxWindowID winid);

		void SetRGB(Img::Color col);
		Img::Color GetRGB();

	private:
		wxSpinCtrl *m_r, *m_g, *m_b;

		void OnChange(wxSpinEvent& evt);

		enum
		{
			RId = wxID_HIGHEST + 1,
			GId,
			BId
		};

		DECLARE_EVENT_TABLE()
	};
}

#endif
