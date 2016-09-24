#include "dlg_color.h"
#include "registry.h"
#include "wintypes.h"
#include "settings_layout.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

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
		pickerPart->Add(m_colorPicker, wxSizerFlags(1).Expand().Border(wxRIGHT, GetPadding()));

		auto satColumn = new wxBoxSizer(wxVERTICAL);
		satColumn->Add(new wxStaticText(this, wxID_ANY, Win::GetStringWx(SIDSettingsBackgroundColorSaturation)), wxSizerFlags(0));
		satColumn->Add(m_saturation, wxSizerFlags(1));
		pickerPart->Add(satColumn, wxSizerFlags(0).Expand());

		auto previewBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsBackgroundColorPreview));
		m_previewPanel = new ControlColorPreview(previewBox->GetStaticBox(), wxID_ANY, { 0, 0 }, { 100, wxDefaultCoord });
		previewBox->Add(m_previewPanel, StaticBoxInnerPadding(1));


		m_hls = new ControlColorHls(this, HlsId);
		m_rgb = new ControlColorRgb(this, RgbId);

		auto editPart = new wxBoxSizer(wxHORIZONTAL);
		editPart->Add(previewBox, StaticBoxInnerPadding(0));
		editPart->Add(m_rgb, StaticBoxInnerPadding(1));
		editPart->Add(m_hls, StaticBoxInnerPadding(1));

		topSizer->Add(pickerPart, StaticBoxOuterPadding(0));
		topSizer->Add(editPart, StaticBoxOuterPadding(0));

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
