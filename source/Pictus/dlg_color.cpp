#include "dlg_color.h"
#include "res_settings.h"
#include "registry.h"
#include "cnt_colorpick.h"
#include "cnt_colorpreview.h"

#include <boost/scoped_array.hpp>

#include <CommCtrl.h>

namespace App {
	using namespace Intl;

	void SetColor::RegisterWindowClasses() {
		ControlColorPicker::RegisterClass(GetHINSTANCE());
		ControlColorPreview::RegisterClass(GetHINSTANCE());
	}

	void SetColor::PerformOnCreate() {
		m_isUpdating = false;
	}

	bool SetColor::PerformOnInitPage() {
		Caption(App::SIDSettingsBackgroundColor);

		ControlText(IDC_GROUP_COLOR_PREVIEW, SIDSettingsBackgroundColorPreview);
		ControlText(IDC_GROUP_COLOR_RGB, SIDSettingsBackgroundColorRGB);
		ControlText(IDC_GROUP_COLOR_HLS, SIDSettingsBackgroundColorHLS);

		ControlText(IDC_TEXT_COLOR_SAT_SLIDER, SIDSettingsBackgroundColorSaturation);
		ControlText(IDC_TEXT_COLOR_R, SIDSettingsBackgroundColorRed);
		ControlText(IDC_TEXT_COLOR_G, SIDSettingsBackgroundColorGreen);
		ControlText(IDC_TEXT_COLOR_B, SIDSettingsBackgroundColorBlue);
		ControlText(IDC_TEXT_COLOR_HUE, SIDSettingsBackgroundColorHue);
		ControlText(IDC_TEXT_COLOR_LUM, SIDSettingsBackgroundColorLuminance);
		ControlText(IDC_TEXT_COLOR_SAT, SIDSettingsBackgroundColorSaturation);

		// Initialize trackbar for saturation
		SendDlgItemMessage(Handle(), IDC_SLIDER_LUM, TBM_SETRANGE, 1, MAKELONG(0, Img::SatCap * 100));
		SendDlgItemMessage(Handle(), IDC_SLIDER_LUM, TBM_SETPAGESIZE, 0, static_cast<LPARAM>(Img::SatCap * 10));

		// Initialize up/downs
		SendDlgItemMessage(Handle(), IDC_SPIN_HUE, UDM_SETBUDDY, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_HUE), 0);
		SendDlgItemMessage(Handle(), IDC_SPIN_LUM, UDM_SETBUDDY, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_LUM), 0);
		SendDlgItemMessage(Handle(), IDC_SPIN_SAT, UDM_SETBUDDY, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_SAT), 0);
		SendDlgItemMessage(Handle(), IDC_SPIN_HUE, UDM_SETRANGE32, 0, static_cast<LPARAM>(Img::HueCap));
		SendDlgItemMessage(Handle(), IDC_SPIN_LUM, UDM_SETRANGE32, 0, static_cast<LPARAM>(Img::LumCap * 100));
		SendDlgItemMessage(Handle(), IDC_SPIN_SAT, UDM_SETRANGE32, 0, static_cast<LPARAM>(Img::SatCap * 100));

		SendDlgItemMessage(Handle(), IDC_SPIN_R, UDM_SETBUDDY, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_R), 0);
		SendDlgItemMessage(Handle(), IDC_SPIN_G, UDM_SETBUDDY, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_G), 0);
		SendDlgItemMessage(Handle(), IDC_SPIN_B, UDM_SETBUDDY, (WPARAM)GetDlgItem(Handle(), IDC_EDIT_B), 0);
		SendDlgItemMessage(Handle(), IDC_SPIN_R, UDM_SETRANGE32, 0, 255);
		SendDlgItemMessage(Handle(), IDC_SPIN_G, UDM_SETRANGE32, 0, 255);
		SendDlgItemMessage(Handle(), IDC_SPIN_B, UDM_SETRANGE32, 0, 255);

		return true;
	}


	void SetColor::PerformUpdateFromSettings(const Reg::Settings& settings) {
		m_currCol = settings.Render.BackgroundColor.ToDWord();
		SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgSetRGB, m_currCol, 0);
	}


	void SetColor::onWriteSettings(Reg::Settings& settings) {
		settings.Render.BackgroundColor = Img::Color::FromDWord(m_currCol);
	}

	void SetColor::updatePreviewColor() {
		SendMessage(GetDlgItem(Handle(), IDC_COLOR_PREVIEW), ControlColorPreview::MsgSetRGB, 0, SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgGetRGB, 0, 0));
	}

	bool SetColor::PerformOnCommand(int id, WPARAM wp, LPARAM lp) {
		switch(HIWORD(wp)) {
			case EN_UPDATE:
			{
				HWND hwnd = (HWND)lp;
				size_t len = SendMessage(hwnd, EM_LINELENGTH, 0, 0);
				boost::scoped_array<wchar_t> pC(new wchar_t[len + 1]);
				SendMessage(hwnd, EM_GETLINE, 0, reinterpret_cast<LPARAM>(pC.get()));
				pC.get()[len] = 0;
				int val = FromWString<int>(pC.get());
				int nval= val;
				if (val < 0) nval = 0;
				if (hwnd == GetDlgItem(Handle(), IDC_EDIT_HUE)) {
					if (val > Img::HueCap) {
						nval = static_cast<int>(Img::HueCap);
					}
				}
				else if (hwnd == GetDlgItem(Handle(), IDC_EDIT_LUM)) {
					if (val > (100.0f * Img::LumCap)) {
						nval = static_cast<int>(100.0f * Img::LumCap);
					}
				}
				else if (hwnd == GetDlgItem(Handle(), IDC_EDIT_SAT)) {
					if (val > (100.0f * Img::SatCap)) {
						nval = static_cast<int>(100.0f * Img::SatCap);
					}
				}

				if (val != nval) SendMessage((HWND)lp, WM_SETTEXT, 0, (LPARAM)ToWString(nval).c_str());
				return true;
			}
			case EN_CHANGE:
				if (!m_isUpdating) {
					switch(id) {
						case IDC_EDIT_R:
						case IDC_EDIT_G:
						case IDC_EDIT_B:
							{
								DWORD col=(GetEditBoxNum(IDC_EDIT_R)<<16)+(GetEditBoxNum(IDC_EDIT_G)<<8) + GetEditBoxNum(IDC_EDIT_B);
								SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgSetRGB, col, 0);
							}
							break;
						case IDC_EDIT_HUE:
							SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgSetHue, 0, GetEditBoxNum(IDC_EDIT_HUE));
							break;
						case IDC_EDIT_LUM:
							SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgSetLuminance, 0, GetEditBoxNum(IDC_EDIT_LUM));
							break;
						case IDC_EDIT_SAT:
							SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgSetSaturation, 0, GetEditBoxNum(IDC_EDIT_SAT));
							break;
						default:
							return false;
					}
				}
				if (id == IDC_EDIT_SAT)
					SendDlgItemMessage(Handle(), IDC_SLIDER_LUM, TBM_SETPOS, 1, 100 - GetEditBoxNum(IDC_EDIT_SAT));

				return true;
		}
		return false;
	}

	bool SetColor::PerformOnVScroll(WPARAM, LPARAM lp) {
		if ((HWND)lp != GetDlgItem(Handle(), IDC_SLIDER_LUM))
			return false;

		int pos=(int)SendDlgItemMessage(Handle(), IDC_SLIDER_LUM, TBM_GETPOS, 0, 0);
		SetEditBoxNum(IDC_EDIT_SAT, 100 - pos);
		SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgSetSaturation, 0, 100 - pos);
		return true;
	}

	bool SetColor::PerformOnApp(int index, WPARAM, LPARAM) {
		switch(index) {
			case ControlColorPicker::NotValueChanged:
				// The value has changed. Update all controls
				m_isUpdating = true;		// Avoid evil recursive loop of death.

				// Get the new color
				uint32_t rgb = static_cast<uint32_t>(SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgGetRGB, 0, 0));
				updatePreviewColor();	// Update color preview

				// Update edit boxes. Only update the one of interest
				SetEditBoxNum(IDC_EDIT_HUE, static_cast<DWORD>(SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgGetHue, 0, 0)));
				SetEditBoxNum(IDC_EDIT_LUM, static_cast<DWORD>(SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgGetLuminance, 0, 0)));
				SetEditBoxNum(IDC_EDIT_SAT, static_cast<DWORD>(SendDlgItemMessage(Handle(), IDC_COLOR_PICK, ControlColorPicker::MsgGetSaturation, 0, 0)));

				BYTE r = (BYTE)((rgb & 0xff0000) >> 16);
				BYTE g = (BYTE)((rgb & 0xff00) >> 8);
				BYTE b = (BYTE)((rgb & 0xff));
				SetEditBoxNum(IDC_EDIT_R, r);
				SetEditBoxNum(IDC_EDIT_G, g);
				SetEditBoxNum(IDC_EDIT_B, b);

				m_isUpdating = false;

				// Store value
				m_currCol = rgb;
				return true;
		}
		return false;
	}

	SetColor::SetColor():
		App::SettingsPage(IDD_SET_COLOR)
	{}

	bool SetColor::IsRootPage() const {
		return false;
	}
}
