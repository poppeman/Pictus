#ifndef DLG_COLOR_H
#define DLG_COLOR_H

#include "settings_page.h"
#include "cnt_color_hls.h"
#include "cnt_color_rgb.h"
#include "cnt_colorpick.h"
#include "cnt_colorpreview.h"
#include <wx/slider.h>

namespace App
{
	class SetColor:public App::SettingsPage
	{
	public:
		bool IsRootPage() const override;
		SetColor(wxWindow *parent);
		std::string Caption() override;

	private:
		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		void OnSaturationEvent(wxCommandEvent& evt);
		void OnColorPicker(wxCommandEvent& evt);
		void OnRgb(wxCommandEvent& evt);
		void OnHls(wxCommandEvent& evt);

		void UpdateControls();

	private:
		ControlColorPicker* m_colorPicker;
		wxSlider* m_saturation;
		ControlColorPreview* m_previewPanel;

		ControlColorRgb* m_rgb;
		ControlColorHls* m_hls;

		enum
		{
			SaturationId = wxID_HIGHEST + 1,
			ColorPickerId,
			RgbId,
			HlsId
		};

		DECLARE_EVENT_TABLE()
	};
}

#endif
