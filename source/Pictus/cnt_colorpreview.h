#ifndef CNT_COLORPREVIEW_H
#define CNT_COLORPREVIEW_H

#include <wx/panel.h>
#include <illa/color.h>

namespace App
{
	class ControlColorPreview:public wxPanel
	{
	public:
		void OnSysColor(wxSysColourChangedEvent& evt);

		void SetColor(Img::Color color);
		ControlColorPreview(wxWindow *parent, wxWindowID winid, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

		DECLARE_EVENT_TABLE()
	};
}

#endif
