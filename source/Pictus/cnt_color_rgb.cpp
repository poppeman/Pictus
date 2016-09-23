#include "cnt_color_rgb.h"
#include "app_types.h"
#include "wintypes.h"

#include "settings_layout.h"

#include <wx/statbox.h>
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
		wxPanel(parent, winid)
	{
		auto box = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsBackgroundColorRGB));
		auto sizer = new  wxFlexGridSizer(2, 5, 5);
		sizer->AddGrowableCol(1, 1);
		box->Add(sizer, StaticBoxInnerPadding(1));
		m_r = new wxSpinCtrl(box->GetStaticBox(), RId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, 255, 0);
		m_g = new wxSpinCtrl(box->GetStaticBox(), GId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, 255, 0);
		m_b = new wxSpinCtrl(box->GetStaticBox(), BId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, 255, 0);
		sizer->Add(new wxStaticText(box->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorRed)));
		sizer->Add(m_r, wxSizerFlags(1).Expand());
		sizer->Add(new wxStaticText(box->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorGreen)));
		sizer->Add(m_g, wxSizerFlags(1).Expand());
		sizer->Add(new wxStaticText(box->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorBlue)));
		sizer->Add(m_b, wxSizerFlags(1).Expand());

		SetSizerAndFit(box);
	}

	void ControlColorRgb::SetRGB(Img::Color col)
	{
		m_r->SetValue(col.R);
		m_g->SetValue(col.G);
		m_b->SetValue(col.B);
		Update();
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
