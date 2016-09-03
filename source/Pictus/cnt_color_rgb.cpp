#include "cnt_color_rgb.h"
#include "app_types.h"
#include "wintypes.h"

#include <wx/sizer.h>
#include <wx/stattext.h>

wxDEFINE_EVENT(COLOR_CHANGED_RGB, wxCommandEvent);

namespace App
{
	BEGIN_EVENT_TABLE(ControlColorRgb, wxStaticBox)
			EVT_SPINCTRL(RId, ControlColorRgb::OnChange)
			EVT_SPINCTRL(GId, ControlColorRgb::OnChange)
			EVT_SPINCTRL(BId, ControlColorRgb::OnChange)
	END_EVENT_TABLE()

	ControlColorRgb::ControlColorRgb(wxWindow* parent, wxWindowID winid):
		wxStaticBox(parent, winid, Win::GetStringWx(SIDSettingsBackgroundColorRGB))
	{
		auto sizer = new  wxFlexGridSizer(2, 5, 5);
		m_r = new wxSpinCtrl(this, RId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, 255, 0);
		m_g = new wxSpinCtrl(this, GId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, 255, 0);
		m_b = new wxSpinCtrl(this, BId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, 255, 0);
		sizer->Add(new wxStaticText(this, wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorRed)));
		sizer->Add(m_r, wxSizerFlags(0));
		sizer->Add(new wxStaticText(this, wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorGreen)));
		sizer->Add(m_g, wxSizerFlags(0));
		sizer->Add(new wxStaticText(this, wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorBlue)));
		sizer->Add(m_b, wxSizerFlags(0));

		SetSizerAndFit(sizer);
	}

	void ControlColorRgb::SetRGB(Img::Color col)
	{
		m_r->SetValue(col.R);
		m_g->SetValue(col.G);
		m_b->SetValue(col.B);
	}

	Img::Color ControlColorRgb::GetRGB()
	{
		return Img::Color{ 0, m_r->GetValue(), m_g->GetValue(), m_b->GetValue() };
	}

	void ControlColorRgb::OnChange(wxSpinEvent &evt)
	{
		auto cmdEvt = new wxCommandEvent(COLOR_CHANGED_RGB, GetId());
		QueueEvent(cmdEvt);
	}
}
