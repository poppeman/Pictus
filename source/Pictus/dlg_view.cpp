#include "dlg_view.h"
#include "registry.h"
#include "wintypes.h"
#include <wx/sizer.h>
#include <wx/statbox.h>

namespace App
{
	const int Padding = 10;

	std::string SetView::Caption()
	{
		return Intl::GetString(App::SIDSettingsViewer);
	}

	/*void SetView::ToggleResizeWindow()
	{
		DoToggleResizeWindow(IsDlgButtonChecked(Handle(), IDC_SIZETOIMAGE) != 0);
	}*/

	void SetView::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_cbDefaultZoom->SetSelection(settings.View.DefaultZoomMode);
		m_cbResizeBehavior->SetSelection(settings.View.ResizeBehaviour);
		m_cbMagFilter->SetSelection(static_cast<int>(settings.Render.MagFilter));
		m_cbMinFilter->SetSelection(static_cast<int>(settings.Render.MinFilter));

		m_wrapAround->SetValue(settings.View.BrowseWrapAround);
		m_resetZoom->SetValue(settings.View.ResetZoom);
		m_resetPan->SetValue(settings.View.ResetPan);
		m_adaptWindowSize->SetValue(settings.View.ResizeWindow);

		DoToggleResizeWindow(settings.View.ResizeWindow);

		// Position method
		auto posmethod = settings.View.ResizePositionMethod;
		if (posmethod == ResizePositionMethod::PositionToScreen)
		{
			m_positionMethod->SetSelection(0);
		}

		if (posmethod == ResizePositionMethod::PositionToCurrent)
		{
			m_positionMethod->SetSelection(1);
		}

		if (posmethod == ResizePositionMethod::PositionNothing)
		{
			m_positionMethod->SetSelection(2);
		}
	}

	void SetView::onWriteSettings(Reg::Settings &settings)
	{
		settings.View.BrowseWrapAround = m_wrapAround->GetValue();
		settings.View.ResetZoom = m_resetZoom->GetValue();
		settings.View.ResetPan = m_resetPan->GetValue();
		settings.View.ResizeWindow = m_adaptWindowSize->GetValue();
		settings.Render.MinFilter = Filter::Mode(m_cbMinFilter->GetSelection());
		settings.Render.MagFilter = Filter::Mode(m_cbMagFilter->GetSelection());

		settings.View.DefaultZoomMode = App::ZoomMode(m_cbDefaultZoom->GetSelection());
		settings.View.ResizeBehaviour = App::ResizeBehaviour(m_cbResizeBehavior->GetSelection());

		auto posmethod = settings.View.ResizePositionMethod;
		switch(m_positionMethod->GetSelection()) {
			case 0:
				posmethod = ResizePositionMethod::PositionToScreen;
				break;
			case 1:
				posmethod = ResizePositionMethod::PositionToCurrent;
				break;
			case 2:
				posmethod = ResizePositionMethod::PositionNothing;
				break;
		}

		settings.View.ResizePositionMethod = posmethod;
	}

	void SetView::DoToggleResizeWindow(bool newState)
	{
		/*SetCheckBox(IDC_SIZETOIMAGE, newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_SCREEN), newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_CURRENT), newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_NOTHING), newState);*/
	}

	wxChoice* SetView::SetupFilterBox(wxWindow* parent)
	{
		wxArrayString choices;
		choices.Add(Win::GetStringWx(SIDResampleModeNearestNeighbor));
		choices.Add(Win::GetStringWx(SIDResampleModeBilinear));
		choices.Add(Win::GetStringWx(SIDResampleModeLanczos3));
		return new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	}

	SetView::SetView(wxWindow* parent) :
		App::SettingsPage(parent),
		m_cbDefaultZoom{nullptr},
		m_cbResizeBehavior{nullptr},
		m_cbMinFilter{nullptr},
		m_cbMagFilter{nullptr}
	{
		//CreateButton(IDC_SIZETOIMAGE)->OnClick.connect([this]() { ToggleResizeWindow(); });
		/*m_cbDefaultZoom->Reset();
		m_cbDefaultZoom->AddItem(SIDActionZoomFree, ZoomFree);
		m_cbDefaultZoom->AddItem(SIDActionZoomFitImage, ZoomFitImage);*/

		auto topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(CreateZoomBox(), wxSizerFlags(0).Expand().Border(wxBOTTOM, Padding / 2));
		topSizer->Add(CreateViewerBox(), wxSizerFlags(0).Expand().Border(wxTOP, Padding / 2));
		SetSizerAndFit(topSizer);
	}

	wxSizer* SetView::CreateZoomBox()
	{
		auto topmostBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsViewerZoom));
		m_resetZoom = new wxCheckBox(topmostBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerResetZoom)));

		topmostBox->Add(CreateResizeAlgoSizer(topmostBox->GetStaticBox()), wxSizerFlags(0).Expand().Border(wxALL, Padding));
		topmostBox->Add(m_resetZoom, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT, Padding));
		topmostBox->Add(CreateModeSizer(topmostBox->GetStaticBox()), wxSizerFlags(0).Expand().Border(wxALL, Padding));

		return topmostBox;
	}


	wxSizer *SetView::CreateResizeAlgoSizer(wxWindow* parent)
	{
		auto topmostSizer= new wxBoxSizer(wxHORIZONTAL);
		auto magBox= new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerMagnification));
		auto minBox= new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerMinification));
		m_cbMagFilter = SetupFilterBox(magBox->GetStaticBox());
		magBox->Add(m_cbMagFilter, wxSizerFlags(1).Expand().Border(wxALL, Padding));
		m_cbMinFilter = SetupFilterBox(minBox->GetStaticBox());
		minBox->Add(m_cbMinFilter, wxSizerFlags(1).Expand().Border(wxALL, Padding));
		topmostSizer->Add(magBox, wxSizerFlags(1).Expand().Border(wxRIGHT, Padding / 2));
		topmostSizer->Add(minBox, wxSizerFlags(1).Expand().Border(wxLEFT, Padding / 2));
		return topmostSizer;
	}

	wxSizer *SetView::CreateModeSizer(wxWindow* parent)
	{
		auto topmostSizer = new wxBoxSizer(wxHORIZONTAL);
		auto defaultZoomModeBoxer = new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerDefaultZoomMode));
		auto resizeBox = new wxStaticBoxSizer(wxVERTICAL, parent, Win::GetStringWx(SIDSettingsViewerResizeBehavior));

		wxArrayString zoomChoices;
		zoomChoices.Add(Win::GetStringWx(SIDActionZoomFree));
		zoomChoices.Add(Win::GetStringWx(SIDActionZoomFitImage));
		m_cbDefaultZoom = new wxChoice(defaultZoomModeBoxer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, zoomChoices);
		defaultZoomModeBoxer->Add(m_cbDefaultZoom, wxSizerFlags(1).Expand().Border(wxALL, Padding));

		wxArrayString behaviors;
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerReduceOrEnlarge));
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerEnlargeOnly));
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerReduceOnly));
		m_cbResizeBehavior = new wxChoice(resizeBox->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, behaviors);
		resizeBox->Add(m_cbResizeBehavior, wxSizerFlags(1).Expand().Border(wxALL, Padding));

		topmostSizer->Add(defaultZoomModeBoxer, wxSizerFlags(1).Expand().Border(wxRIGHT, Padding / 2));
		topmostSizer->Add(resizeBox, wxSizerFlags(1).Expand().Border(wxLEFT, Padding / 2));
		return topmostSizer;
	}

	wxSizer *SetView::CreateViewerBox()
	{
		auto viewerBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsViewer));

		wxArrayString positionMethods;
		positionMethods.Add(wxString::FromUTF8(Intl::GetString(SIDSettingsViewerCenterScreen)));
		positionMethods.Add(wxString::FromUTF8(Intl::GetString(SIDSettingsViewerCenterPosition)));
		positionMethods.Add(wxString::FromUTF8(Intl::GetString(SIDSettingsViewerNoReposition)));
		m_positionMethod = new wxRadioBox(viewerBox->GetStaticBox(), wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, positionMethods, 0, wxVERTICAL);

		m_wrapAround = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerWrapAround)));
		m_resetPan = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerResetPan)));
		m_adaptWindowSize = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsViewerAdaptWindowSize));

		viewerBox->Add(m_wrapAround, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT|wxTOP, Padding));
		viewerBox->Add(m_resetPan, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT, Padding));
		viewerBox->Add(m_adaptWindowSize, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT, Padding));
		viewerBox->Add(m_positionMethod, wxSizerFlags(0).Expand().Border(wxLEFT|wxRIGHT|wxBOTTOM, Padding));

		return viewerBox;
	}
}
