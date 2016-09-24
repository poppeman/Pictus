#include "cnt_colorpreview.h"
#include "wintypes.h"

namespace App
{
	BEGIN_EVENT_TABLE(ControlColorPreview, wxPanel)
		EVT_SYS_COLOUR_CHANGED(ControlColorPreview::OnSysColor)
	END_EVENT_TABLE()

	void ControlColorPreview::SetColor(Img::Color color)
	{
		SetBackgroundColour(Win::ColorToWx(color));
		Refresh();
		Update();
	}

	ControlColorPreview::ControlColorPreview(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size):
		wxPanel{parent, winid, pos, size}
	{}

	void ControlColorPreview::OnSysColor(wxSysColourChangedEvent& evt)
	{
		evt.Skip(true);
	}
}
