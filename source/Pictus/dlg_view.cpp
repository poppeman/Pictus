#include "dlg_view.h"
#include "registry.h"
#include "res_settings.h"
#include "orz/intl.h"
#include "ctrl_button.h"

namespace App {
	using namespace Intl;

	void SetView::ToggleResizeWindow() {
		DoToggleResizeWindow(IsDlgButtonChecked(Handle(), IDC_SIZETOIMAGE)!=0);
	}

	bool SetView::PerformOnInitPage() {
		CreateButton(IDC_SIZETOIMAGE)->OnClick.connect([this]() { ToggleResizeWindow(); });
		Caption(App::SIDSettingsViewer);

		m_cbDefaultZoom = CreateComboBox(IDC_COMBO_VIEWER_DEFAULTZOOM);
		m_cbResizeBehavior = CreateComboBox(IDC_COMBO_VIEWER_RESIZEBEHAVIOUR);
		m_cbMinFilter = CreateComboBox(IDC_COMBO_MIN);
		m_cbMagFilter = CreateComboBox(IDC_COMBO_MAG);

		ControlText(IDC_GROUP_ZOOMING, SIDSettingsViewerZoom);
		ControlText(IDC_GROUP_MAG, SIDSettingsViewerMagnification);
		ControlText(IDC_GROUP_MIN, SIDSettingsViewerMinification);
		ControlText(IDC_GROUP_VIEWER, SIDSettingsViewer);
		ControlText(IDC_RESETZOOM, SIDSettingsViewerResetZoom);
		ControlText(IDC_RESETPAN, SIDSettingsViewerResetPan);
		ControlText(IDC_SIZETOIMAGE, SIDSettingsViewerAdaptWindowSize);
		ControlText(IDC_SIZETO_SCREEN, SIDSettingsViewerCenterScreen);
		ControlText(IDC_SIZETO_CURRENT, SIDSettingsViewerCenterPosition);
		ControlText(IDC_SIZETO_NOTHING, SIDSettingsViewerNoReposition);
		ControlText(IDC_CHECK_VIEWER_WRAPAROUND, SIDSettingsViewerWrapAround);

		ControlText(IDC_GROUP_VIEWER_DEFAULTZOOM, SIDSettingsViewerDefaultZoomMode);

		m_cbDefaultZoom->Reset();
		m_cbDefaultZoom->AddItem(SIDActionZoomFree, ZoomFree);
		m_cbDefaultZoom->AddItem(SIDActionZoomFitImage, ZoomFitImage);

		ControlText(IDC_GROUP_VIEWER_RESIZEBEHAVIOUR, SIDSettingsViewerResizeBehavior);
		m_cbResizeBehavior->Reset();
		m_cbResizeBehavior->AddItem(SIDSettingsViewerReduceOrEnlarge, ResizeEnlargeOrReduce);
		m_cbResizeBehavior->AddItem(SIDSettingsViewerEnlargeOnly, ResizeEnlargeOnly);
		m_cbResizeBehavior->AddItem(SIDSettingsViewerReduceOnly, ResizeReduceOnly);

		SetupFilterBox(m_cbMagFilter);

		SetupFilterBox(m_cbMinFilter);

		return true;
	}

	void SetView::PerformUpdateFromSettings(const Reg::Settings& settings) {
		m_cbDefaultZoom->SetSelection(settings.View.DefaultZoomMode);
		m_cbResizeBehavior->SetSelection(settings.View.ResizeBehaviour);
		m_cbMagFilter->SetSelection(static_cast<int>(settings.Render.MagFilter));
		m_cbMinFilter->SetSelection(static_cast<int>(settings.Render.MinFilter));

		SetCheckBox(IDC_CHECK_VIEWER_WRAPAROUND, settings.View.BrowseWrapAround);
		SetCheckBox(IDC_RESETZOOM, settings.View.ResetZoom);
		SetCheckBox(IDC_RESETPAN, settings.View.ResetPan);
		DoToggleResizeWindow(settings.View.ResizeWindow);

		// Position method
		auto posmethod = static_cast<ResizePositionMethod>(settings.View.ResizePositionMethod);
		if (posmethod == ResizePositionMethod::PositionToScreen) {
			SetCheckBox(IDC_SIZETO_SCREEN, true);
		}

		if (posmethod == ResizePositionMethod::PositionToCurrent) {
			SetCheckBox(IDC_SIZETO_CURRENT, true);
		}

		if (posmethod == ResizePositionMethod::PositionNothing) {
			SetCheckBox(IDC_SIZETO_NOTHING, true);
		}

	}


	void SetView::onWriteSettings(Reg::Settings& settings) {
		settings.View.BrowseWrapAround = GetCheckBox(IDC_CHECK_VIEWER_WRAPAROUND);
		settings.View.ResetZoom = GetCheckBox(IDC_RESETZOOM);
		settings.View.ResetPan = GetCheckBox(IDC_RESETPAN);
		settings.View.ResizeWindow = GetCheckBox(IDC_SIZETOIMAGE);
		settings.Render.MinFilter = Filter::Mode(m_cbMinFilter->GetSelectionData());
		settings.Render.MagFilter = Filter::Mode(m_cbMagFilter->GetSelectionData());

		settings.View.DefaultZoomMode = App::ZoomMode(m_cbDefaultZoom->GetSelectionData());
		settings.View.ResizeBehaviour = App::ResizeBehaviour(m_cbResizeBehavior->GetSelectionData());

		auto posmethod = settings.View.ResizePositionMethod;
		if (IsDlgButtonChecked(Handle(), IDC_SIZETO_SCREEN)) {
			posmethod = ResizePositionMethod::PositionToScreen;
		}
		else if (IsDlgButtonChecked(Handle(), IDC_SIZETO_CURRENT)) {
			posmethod = ResizePositionMethod::PositionToCurrent;
		}
		else if (IsDlgButtonChecked(Handle(), IDC_SIZETO_NOTHING)) {
			posmethod = ResizePositionMethod::PositionNothing;
		}

		settings.View.ResizePositionMethod = posmethod;
	}

	void SetView::DoToggleResizeWindow(bool newState) {
		SetCheckBox(IDC_SIZETOIMAGE, newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_SCREEN), newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_CURRENT), newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_NOTHING), newState);
	}

	void SetView::SetupFilterBox(Win::ComboBox* ctrl) {
		ctrl->Reset(); //SendDlgItemMessage(Handle(), id, CB_RESETCONTENT, 0, 0); 
		ctrl->AddItem(SIDResampleModeNearestNeighbor, static_cast<int>(Filter::Mode::NearestNeighbor));
		ctrl->AddItem(SIDResampleModeBilinear, static_cast<int>(Filter::Mode::Bilinear));
		ctrl->AddItem(SIDResampleModeLanczos3, static_cast<int>(Filter::Mode::Lanczos3));
	}

	SetView::SetView():
		App::SettingsPage(IDD_SET_VIEWER),
		m_cbDefaultZoom{ nullptr },
		m_cbMagFilter{ nullptr },
		m_cbMinFilter{ nullptr },
		m_cbResizeBehavior{ nullptr }
	{}
}
