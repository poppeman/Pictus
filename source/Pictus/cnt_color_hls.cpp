#include "cnt_color_hls.h"
#include "app_types.h"
#include "wintypes.h"

#include <wx/statbox.h>
#include <wx/sizer.h>

wxDEFINE_EVENT(COLOR_CHANGED_HLS, wxCommandEvent);

namespace App
{
	BEGIN_EVENT_TABLE(ControlColorHls, wxStaticBox)
		EVT_SPINCTRL(HId, ControlColorHls::OnChange)
		EVT_SPINCTRL(LId, ControlColorHls::OnChange)
		EVT_SPINCTRL(SId, ControlColorHls::OnChange)
    END_EVENT_TABLE()

	ControlColorHls::ControlColorHls(wxWindow* parent, wxWindowID winid):
		wxStaticBox(parent, winid, Win::GetStringWx(SIDSettingsBackgroundColorHLS))
	{
		auto sizer = new wxBoxSizer(wxVERTICAL);

		m_h = new wxSpinCtrl(this, HId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, Img::HueCap, 0);
		m_l = new wxSpinCtrl(this, LId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, Img::LumCap * 100, 0);
		m_s = new wxSpinCtrl(this, SId, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, 0, Img::SatCap * 100, 0);
		sizer->Add(m_h, wxSizerFlags(0));
		sizer->Add(m_l, wxSizerFlags(0));
		sizer->Add(m_s, wxSizerFlags(0));

		SetSizerAndFit(sizer);
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
