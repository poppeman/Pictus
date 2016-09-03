#include "dlg_color.h"
#include "registry.h"
#include "wintypes.h"
#include <wx/sizer.h>

namespace App
{
	using namespace Intl;

	BEGIN_EVENT_TABLE(SetColor, wxPanel)
		EVT_SLIDER(SaturationId, SetColor::OnSaturationEvent)
		EVT_COMMAND(ColorPickerId, COLOR_CHANGED, SetColor::OnColorPicker)
		EVT_COMMAND(HlsId, COLOR_CHANGED_HLS, SetColor::OnHls)
		EVT_COMMAND(RgbId, COLOR_CHANGED_RGB, SetColor::OnRgb)
	END_EVENT_TABLE()

	bool SetColor::IsRootPage() const
	{
		return false;
	}

	std::string SetColor::Caption()
	{
		return GetString(SIDSettingsBackgroundColor);
	}

	SetColor::SetColor(wxWindow* parent):
		App::SettingsPage{parent}
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);

		auto pickerPart = new wxBoxSizer(wxHORIZONTAL);
		m_colorPicker = new ControlColorPicker(this, ColorPickerId, {0, 0}, {256, 256});
		m_saturation = new wxSlider(this, SaturationId, 1, 1, Img::SatCap * 100, wxDefaultPosition, wxDefaultSize, wxVERTICAL);
		pickerPart->Add(m_colorPicker, wxSizerFlags(0));
		pickerPart->Add(m_saturation, wxSizerFlags(0).Expand().Right());

		auto previewBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsBackgroundColorPreview));
		m_previewPanel = new ControlColorPreview(previewBox->GetStaticBox(), wxID_ANY, {0, 0}, {80, 128});
		previewBox->Add(m_previewPanel, wxSizerFlags(0));


		m_hls = new ControlColorHls(this, HlsId);
		m_rgb = new ControlColorRgb(this, RgbId);

		auto editPart = new wxBoxSizer(wxHORIZONTAL);
		editPart->Add(previewBox, wxSizerFlags(0));
		editPart->Add(m_rgb, wxSizerFlags(0).Expand());
		editPart->Add(m_hls, wxSizerFlags(0).Expand());

		topSizer->Add(pickerPart, wxSizerFlags(0));
		topSizer->Add(editPart);

		SetSizerAndFit(topSizer);
	}


	void SetColor::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_colorPicker->SetRgb(settings.Render.BackgroundColor);
	}


	void SetColor::onWriteSettings(Reg::Settings &settings)
	{
		settings.Render.BackgroundColor = m_colorPicker->GetRgb();
	}

	void SetColor::OnSaturationEvent(wxCommandEvent& evt)
	{
		auto hls = m_colorPicker->GetHls();
		hls.S = m_saturation->GetValue() / 100.0f;
		m_colorPicker->SetHls(hls);
		UpdateControls();
	}

	void SetColor::OnColorPicker(wxCommandEvent &evt)
	{
		UpdateControls();
	}

	void SetColor::OnRgb(wxCommandEvent &evt)
	{
		m_colorPicker->SetRgb(m_rgb->GetRGB());
	}

	void SetColor::OnHls(wxCommandEvent &evt)
	{
		m_colorPicker->SetHls(m_hls->GetHls());
	}

	void SetColor::UpdateControls()
	{
		auto hls = m_colorPicker->GetHls();
		m_rgb->SetRGB(m_colorPicker->GetRgb());
		m_hls->SetHls(hls);
		m_previewPanel->SetColor(m_colorPicker->GetRgb());
		m_saturation->SetValue((int) (hls.S * 100));
	}
}
