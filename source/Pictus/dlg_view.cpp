#include "dlg_view.h"
#include "registry.h"
#include "wintypes.h"
#include <wx/sizer.h>
#include <wx/statbox.h>

namespace App
{
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

		DoToggleResizeWindow(settings.View.ResizeWindow);

		// Position method
		auto posmethod = static_cast<ResizePositionMethod>(settings.View.ResizePositionMethod);
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
		settings.View.ResizeWindow = m_sizeToImage->GetValue();
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

		/*ctrl->Reset(); //SendDlgItemMessage(Handle(), id, CB_RESETCONTENT, 0, 0);
		ctrl->AddItem(, static_cast<int>(Filter::Mode::NearestNeighbor));
		ctrl->AddItem(, static_cast<int>(Filter::Mode::Bilinear));
		ctrl->AddItem(, static_cast<int>(Filter::Mode::Lanczos3));*/
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


		auto zoomModeBox = new wxStaticBox(this, wxID_ANY, Win::GetStringWx(SIDSettingsViewerDefaultZoomMode));
		wxArrayString zoomChoices;
		zoomChoices.Add(Intl::GetString(SIDActionZoomFree));
		zoomChoices.Add(Intl::GetString(SIDActionZoomFitImage));
		m_cbDefaultZoom = new wxChoice(zoomModeBox, wxID_ANY, wxDefaultPosition, wxDefaultSize, zoomChoices);




		/*CreateComboBox(IDC_COMBO_VIEWER_DEFAULTZOOM);
		m_cbResizeBehavior = CreateComboBox(IDC_COMBO_VIEWER_RESIZEBEHAVIOUR);
		m_cbMinFilter = CreateComboBox(IDC_COMBO_MIN);
		m_cbMagFilter = CreateComboBox(IDC_COMBO_MAG);*/

		auto viewerBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsViewer));

		wxArrayString positionMethods;
		positionMethods.Add(wxString::FromUTF8(Intl::GetString(SIDSettingsViewerCenterScreen)));
		positionMethods.Add(wxString::FromUTF8(Intl::GetString(SIDSettingsViewerCenterPosition)));
		positionMethods.Add(wxString::FromUTF8(Intl::GetString(SIDSettingsViewerNoReposition)));
		m_positionMethod = new wxRadioBox(viewerBox->GetStaticBox(), wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, positionMethods, 0, wxVERTICAL);

		m_wrapAround = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerWrapAround)));
		m_resetZoom = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerResetZoom)));
		m_resetPan = new wxCheckBox(viewerBox->GetStaticBox(), wxID_ANY, wxString::FromUTF8(Intl::GetString(SIDSettingsViewerResetPan)));
		viewerBox->Add(m_wrapAround, wxSizerFlags(0).Expand());
		viewerBox->Add(m_resetZoom, wxSizerFlags(0).Expand());
		viewerBox->Add(m_resetPan, wxSizerFlags(0).Expand());
		viewerBox->Add(m_positionMethod, wxSizerFlags(0).Expand());

		auto resizeBox = new wxStaticBox(this, wxID_ANY, Win::GetStringWx(SIDSettingsViewerResizeBehavior));
		wxArrayString behaviors;
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerReduceOrEnlarge));
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerEnlargeOnly));
		behaviors.Add(Win::GetStringWx(SIDSettingsViewerReduceOnly));
		m_cbResizeBehavior = new wxChoice(resizeBox, wxID_ANY, wxDefaultPosition, wxDefaultSize, behaviors);

		auto magBox = new wxStaticBox(this, wxID_ANY, Win::GetStringWx(SIDSettingsViewerMagnification));
		m_cbMagFilter = SetupFilterBox(magBox);
		auto minBox = new wxStaticBox(this, wxID_ANY, Win::GetStringWx(SIDSettingsViewerMinification));
		m_cbMinFilter = SetupFilterBox(minBox);

		auto resizeAlgoSizer = new wxBoxSizer(wxHORIZONTAL);
		resizeAlgoSizer->Add(magBox, wxSizerFlags(1).Expand());
		resizeAlgoSizer->Add(minBox, wxSizerFlags(1).Expand());

		auto modeSizer = new wxBoxSizer(wxHORIZONTAL);
		modeSizer->Add(zoomModeBox, wxSizerFlags(1).Expand());
		modeSizer->Add(resizeBox, wxSizerFlags(1).Expand());

		auto zoomBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsViewerZoom));
		zoomBox->Add(resizeAlgoSizer, wxSizerFlags(1).Expand());
		zoomBox->Add(m_resetZoom, wxSizerFlags(0).Expand());
		zoomBox->Add(modeSizer, wxSizerFlags(1).Expand());

		auto topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(zoomBox, wxSizerFlags(1).Expand());
		topSizer->Add(viewerBox, wxSizerFlags(1).Expand());
		SetSizerAndFit(topSizer);
	}
}
