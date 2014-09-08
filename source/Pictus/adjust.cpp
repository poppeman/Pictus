#include "StdAfx.h"
#include "adjust.h"
#include "res_viewer.h"
#include "orz/intl.h"
#include "illa/config.h"
#include "ctrl_button.h"

using namespace Intl;

namespace App {
	bool Adjust::PerformOnInitDialog() {
		auto applyFunc = [this]() { Apply(); };
		auto closeFunc = [this]() { Close(); };
		auto defaultFunc = [this]() { Default(); };

		CreateButton(IDC_BUTTON_ADJUST_APPLY)->OnClick.connect(applyFunc);
		CreateButton(IDC_CHECK_ADJUST_AUTOPROOF)->OnClick.connect(applyFunc);
		CreateButton(IDC_BUTTON_ADJUST_CLOSE)->OnClick.connect(closeFunc);
		CreateButton(IDC_BUTTON_ADJUST_DEFAULT)->OnClick.connect(defaultFunc);

		Caption(SIDAdjust);
		ControlText(IDC_GROUP_ADJUST_BRIGHTNESS, SIDGroupBrightness);
		ControlText(IDC_GROUP_ADJUST_CONTRAST, SIDGroupContrast);
		ControlText(IDC_GROUP_ADJUST_GAMMA, SIDGroupGamma);
		ControlText(IDC_CHECK_ADJUST_AUTOPROOF, SIDAutoProof);
		ControlText(IDC_BUTTON_ADJUST_APPLY, SIDApply);
		ControlText(IDC_BUTTON_ADJUST_CLOSE, SIDClose);
		ControlText(IDC_BUTTON_ADJUST_DEFAULT, SIDDefault);

		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_SETRANGE, 1, MAKELONG(Img::MinBrightness, Img::MaxBrightness));
		//SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_SETPAGESIZE, 0, (MaxBrightness-MinBrightness)/100);
		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_SETPOS, 1, 0);
		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_SETTICFREQ, (Img::MaxBrightness-Img::MinBrightness)/10, 0);

		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_SETRANGE, 1, MAKELONG(Img::MinContrast, Img::MaxContrast));
		//SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_SETPAGESIZE, 0, (MaxContrast-MinContrast)/100);
		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_SETPOS, 1, Img::ContrastStep);
		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_SETTICFREQ, (Img::MaxContrast-Img::MinContrast)/10, 0);

		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_SETRANGE, 1, MAKELONG(Img::MinGamma, Img::MaxGamma));
		//SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_SETPAGESIZE, 0, (MaxGamma-MinGamma)/100);
		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_SETPOS, 1, 10);
		SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_SETTICFREQ, (Img::MaxGamma-Img::MinGamma)/10, 0);

		return true;
	}

	bool Adjust::PerformOnHScroll(WPARAM, LPARAM) {
		if(!isAutoProof()) return false;
		Apply();
		return true;
	}

	void Adjust::Brightness(int newBright) {
		if (newBright != SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_GETPOS, 0, 0))
			SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_SETPOS, 1, newBright);
	}

	int Adjust::Brightness() const {
		return static_cast<int>(SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_BRIGHTNESS, TBM_GETPOS, 0, 0));
	}

	void Adjust::Contrast(int newContrast) {
		if (newContrast!=SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_GETPOS, 0, 0))
			SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_SETPOS, 1, newContrast);
	}

	int Adjust::Contrast() const {
		return static_cast<int>(SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_CONTRAST, TBM_GETPOS, 0, 0));
	}

	void Adjust::Gamma(int newGamma) {
		if (newGamma!=SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_GETPOS, 0, 0))
			SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_SETPOS, 1, newGamma);
	}

	int Adjust::Gamma() const {
		return static_cast<int>(SendDlgItemMessage(Handle(), IDC_SLIDER_ADJUST_GAMMA, TBM_GETPOS, 0, 0));
	}

	Adjust::Adjust():Win::Dialog(IDD_ADJUST) {}

	bool Adjust::isAutoProof() {
		return GetCheckBox(IDC_CHECK_ADJUST_AUTOPROOF);
	}

	void Adjust::Apply() {
		OnChange(Brightness(), Contrast(), Gamma());
	}

	void Adjust::Close() {
		DestroyWindow(Handle());
	}

	void Adjust::Default() {
		Brightness(0);
		Contrast(10);
		Gamma(10);
		if (isAutoProof())
			Apply();
	}
}
