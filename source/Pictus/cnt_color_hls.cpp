#include "cnt_color_hls.h"
#include "app_types.h"
#include "wintypes.h"

#include "settings_layout.h"

#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

wxDEFINE_EVENT(COLOR_CHANGED_HLS, wxCommandEvent);

namespace App
{
	BEGIN_EVENT_TABLE(ControlColorHls, wxStaticBox)
		EVT_SPINCTRL(HId, ControlColorHls::OnChange)
		EVT_SPINCTRL(LId, ControlColorHls::OnChange)
		EVT_SPINCTRL(SId, ControlColorHls::OnChange)
    END_EVENT_TABLE()

	ControlColorHls::ControlColorHls(wxWindow* parent, wxWindowID winid):
		wxPanel(parent, winid)
	{
		auto box = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsBackgroundColorHLS));
		auto sizer = new  wxFlexGridSizer(2, 5, 5);
		sizer->AddGrowableCol(1, 1);
		box->Add(sizer, StaticBoxInnerPadding(1));
		m_h = new wxSpinCtrl(box->GetStaticBox(), HId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, Img::HueCap, 0);
		m_l = new wxSpinCtrl(box->GetStaticBox(), LId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, Img::LumCap * 100, 0);
		m_s = new wxSpinCtrl(box->GetStaticBox(), SId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, Img::SatCap * 100, 0);
		sizer->Add(new wxStaticText(box->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorHue)));
		sizer->Add(m_h, wxSizerFlags(1).Expand());
		sizer->Add(new wxStaticText(box->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorLuminance)));
		sizer->Add(m_l, wxSizerFlags(1).Expand());
		sizer->Add(new wxStaticText(box->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorSaturation)));
		sizer->Add(m_s, wxSizerFlags(1).Expand());

		SetSizerAndFit(box);
	}

	void ControlColorHls::SetHls(Img::HLSTriplet col)
	{
		m_h->SetValue(col.H);
		m_l->SetValue((int) (col.L * 100));
		m_s->SetValue((int) (col.S * 100));
	}

	Img::HLSTriplet ControlColorHls::GetHls()
	{
		return Img::HLSTriplet{ static_cast<float>(m_h->GetValue()), m_l->GetValue() / 100.0f, m_s->GetValue() / 100.0f };
	}

	void ControlColorHls::OnChange(wxSpinEvent &evt)
	{
		auto cmdEvt = new wxCommandEvent(COLOR_CHANGED_HLS, GetId());
		QueueEvent(cmdEvt);
	}
}
