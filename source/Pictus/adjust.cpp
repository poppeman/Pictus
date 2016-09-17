#include "adjust.h"
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include "wintypes.h"
#include "illa/config.h"

using namespace Intl;

namespace App
{
	void Adjust::Brightness(int newBright)
	{
		if (newBright != m_brightness->GetValue())
		{
			m_brightness->SetValue(newBright);
		}
	}

	int Adjust::Brightness() const
	{
		return m_brightness->GetValue();
	}

	void Adjust::Contrast(int newContrast)
	{
		if (newContrast != m_contrast->GetValue())
		{
			m_contrast->SetValue(newContrast);
		}
	}

	int Adjust::Contrast() const
	{
		return m_contrast->GetValue();
	}

	void Adjust::Gamma(int newGamma)
	{
		if (newGamma != m_gamma->GetValue())
		{
			m_gamma->SetValue(newGamma);
		}
	}

	int Adjust::Gamma() const
	{
		return m_gamma->GetValue();
	}

	Adjust::Adjust(wxWindow* parent):
		wxDialog(parent, wxID_ANY, Win::GetStringWx(SIDAdjust), wxDefaultPosition,wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP)
	{
		auto applyFunc = [this](wxCommandEvent& evt) { Apply(); };
		auto closeFunc = [this](wxCommandEvent& evt) { Close(); };
		auto defaultFunc = [this](wxCommandEvent& evt) { Default(); };

		auto onChange = [this](wxCommandEvent& evt) {
			if (!isAutoProof()) return false;
			Apply();
			return true;
		};

		auto brightnessBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDAdjustBrightness));
		m_brightness = new wxSlider(brightnessBox->GetStaticBox(), wxID_ANY, 0, Img::MinBrightness, Img::MaxBrightness, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_AUTOTICKS);
		brightnessBox->Add(m_brightness, wxSizerFlags(0).Border(wxALL, 10).Expand());
		auto contrastBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDAdjustContrast));
		m_contrast = new wxSlider(contrastBox->GetStaticBox(), wxID_ANY, Img::ContrastStep, Img::MinContrast, Img::MaxContrast, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_AUTOTICKS);
		contrastBox->Add(m_contrast, wxSizerFlags(0).Border(wxALL, 10).Expand());
		auto gammaBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDAdjustGamma));
		m_gamma = new wxSlider(gammaBox->GetStaticBox(), wxID_ANY, 10, Img::MinGamma, Img::MaxGamma, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_AUTOTICKS);
		gammaBox->Add(m_gamma, wxSizerFlags(0).Border(wxALL, 10).Expand());

		m_brightness->SetTickFreq((Img::MaxBrightness - Img::MinBrightness) / 10);
		m_contrast->SetTickFreq((Img::MaxContrast - Img::MinContrast) / 10);
		m_gamma->SetTickFreq((Img::MaxGamma - Img::MinGamma) / 10);

		m_autoApply = new wxCheckBox(this, wxID_ANY, Win::GetStringWx(SIDAdjustAutoApply));
		auto applyButton = new wxButton(this, wxID_ANY, Win::GetStringWx(SIDDialogApply));
		auto defaultButton = new wxButton(this, wxID_ANY, Win::GetStringWx(SIDAdjustDefault));
		auto closeButton = new wxButton(this, wxID_ANY, Win::GetStringWx(SIDDialogClose));

		auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonSizer->Add(applyButton, wxSizerFlags(1));
		buttonSizer->Add(defaultButton, wxSizerFlags(1).Border(wxLEFT | wxRIGHT, 10));
		buttonSizer->Add(closeButton, wxSizerFlags(1));

		m_brightness->Bind(wxEVT_SLIDER, onChange);
		m_contrast->Bind(wxEVT_SLIDER, onChange);
		m_gamma->Bind(wxEVT_SLIDER, onChange);
		m_autoApply->Bind(wxEVT_CHECKBOX, applyFunc);
		applyButton->Bind(wxEVT_BUTTON, applyFunc);
		defaultButton->Bind(wxEVT_BUTTON, defaultFunc);
		closeButton->Bind(wxEVT_BUTTON, closeFunc);

		auto topmostSizer = new wxBoxSizer(wxVERTICAL);
		topmostSizer->Add(brightnessBox, wxSizerFlags(0).Border(wxALL, 10).Expand());
		topmostSizer->Add(contrastBox, wxSizerFlags(0).Border(wxALL, 10).Expand());
		topmostSizer->Add(gammaBox, wxSizerFlags(0).Border(wxALL, 10).Expand());
		topmostSizer->Add(m_autoApply, wxSizerFlags(0).Border(wxALL, 10).Expand());
		topmostSizer->Add(buttonSizer, wxSizerFlags(0).Border(wxALL, 10).Expand());

		SetSizerAndFit(topmostSizer);
	}

	bool Adjust::isAutoProof()
	{
		return m_autoApply->GetValue();
	}

	void Adjust::Apply()
	{
		OnChange(Brightness(), Contrast(), Gamma());
	}

	void Adjust::Default()
	{
		Brightness(0);
		Contrast(10);
		Gamma(10);
		if (isAutoProof())
		{
			Apply();
		}
	}
}
