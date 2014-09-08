#include "StdAfx.h"
#include "dlg_view.h"
#include "registry.h"
#include "res_settings.h"
#include "orz/intl.h"
#include "ctrl_button.h"

namespace App {
	using namespace Reg::Keys;
	using namespace Intl;

	void SetView::ToggleResizeWindow() {
		DoToggleResizeWindow(IsDlgButtonChecked(Handle(), IDC_SIZETOIMAGE)!=0);
	}

	bool SetView::PerformOnInitPage() {
		CreateButton(IDC_SIZETOIMAGE)->OnClick.connect([this]() { ToggleResizeWindow(); });
		Caption(App::SIDViewer);

		m_cbDefaultZoom = CreateComboBox(IDC_COMBO_VIEWER_DEFAULTZOOM);
		m_cbResizeBehavior = CreateComboBox(IDC_COMBO_VIEWER_RESIZEBEHAVIOUR);
		m_cbMinFilter = CreateComboBox(IDC_COMBO_MIN);
		m_cbMagFilter = CreateComboBox(IDC_COMBO_MAG);

		ControlText(IDC_GROUP_ZOOMING, SIDZoom);
		ControlText(IDC_GROUP_MAG, SIDGroupMagnification);
		ControlText(IDC_GROUP_MIN, SIDGroupMinification);
		ControlText(IDC_GROUP_VIEWER, SIDGroupViewer);
		ControlText(IDC_RESETZOOM, SIDResetZoom);
		ControlText(IDC_RESETPAN, SIDResetPan);
		ControlText(IDC_SIZETOIMAGE, SIDResizeWindow);
		ControlText(IDC_SIZETO_SCREEN, SIDWindowCenterScreen);
		ControlText(IDC_SIZETO_CURRENT, SIDWindowCenterCurrent);
		ControlText(IDC_SIZETO_NOTHING, SIDWindowNoReposition);
		ControlText(IDC_CHECK_VIEWER_WRAPAROUND, SIDWrapAround);

		ControlText(IDC_GROUP_VIEWER_DEFAULTZOOM, SIDGroupZM);

		m_cbDefaultZoom->Reset();
		m_cbDefaultZoom->AddItem(SIDZoomFullSize, ZoomFree);
		m_cbDefaultZoom->AddItem(SIDZoomFitImage, ZoomFitImage);
		m_cbDefaultZoom->SetSelection(Reg::Key(DWDefaultZoomMode));

		ControlText(IDC_GROUP_VIEWER_RESIZEBEHAVIOUR, SIDGroupRM);
		m_cbResizeBehavior->Reset();
		m_cbResizeBehavior->AddItem(SIDRMBoth, ResizeEnlargeOrReduce);
		m_cbResizeBehavior->AddItem(SIDRMEnlargeOnly, ResizeEnlargeOnly);
		m_cbResizeBehavior->AddItem(SIDRMReduceOnly, ResizeReduceOnly);
		m_cbResizeBehavior->SetSelection(Reg::Key(DWResizeBehaviour));

		SetupFilterBox(m_cbMagFilter);
		m_cbMagFilter->SetSelection(Reg::Key(DWMagFilter));

		SetupFilterBox(m_cbMinFilter);
		m_cbMinFilter->SetSelection(Reg::Key(DWMinFilter));

		SetCheckBox(IDC_CHECK_VIEWER_WRAPAROUND, Reg::Key(DWBrowseWrapAround) != 0);
		SetCheckBox(IDC_RESETZOOM, Reg::Key(DWResetZoom) != 0);
		SetCheckBox(IDC_RESETPAN,  Reg::Key(DWResetPan) != 0);
		DoToggleResizeWindow(Reg::Key(DWResizeWindow) != 0);

		// Position method
		ResizePositionMethod posmethod = static_cast<ResizePositionMethod>(Reg::Key(DWResizePositionMethod));
		if (posmethod == ResizePositionMethod::PositionToScreen)
			SetCheckBox(IDC_SIZETO_SCREEN, true);

		if (posmethod == ResizePositionMethod::PositionToCurrent)
			SetCheckBox(IDC_SIZETO_CURRENT, true);

		if (posmethod == ResizePositionMethod::PositionNothing)
			SetCheckBox(IDC_SIZETO_NOTHING, true);

		return true;
	}

	void SetView::onWriteSettings() {
		Reg::Key(DWBrowseWrapAround, GetCheckBox(IDC_CHECK_VIEWER_WRAPAROUND));
		Reg::Key(DWResetZoom, GetCheckBox(IDC_RESETZOOM));
		Reg::Key(DWResetPan,  GetCheckBox(IDC_RESETPAN));
		Reg::Key(DWResizeWindow, GetCheckBox(IDC_SIZETOIMAGE));
		Reg::Key(DWMinFilter, m_cbMinFilter->GetSelectionData());
		Reg::Key(DWMagFilter, m_cbMagFilter->GetSelectionData());

		Reg::Key(DWDefaultZoomMode, m_cbDefaultZoom->GetSelectionData());
		Reg::Key(DWResizeBehaviour, m_cbResizeBehavior->GetSelectionData());

		ResizePositionMethod posmethod = static_cast<ResizePositionMethod>(Reg::Key(DWResizePositionMethod));
		if (IsDlgButtonChecked(Handle(), IDC_SIZETO_SCREEN))
			posmethod = ResizePositionMethod::PositionToScreen;
		else if (IsDlgButtonChecked(Handle(), IDC_SIZETO_CURRENT))
			posmethod = ResizePositionMethod::PositionToCurrent;
		else if (IsDlgButtonChecked(Handle(), IDC_SIZETO_NOTHING))
			posmethod = ResizePositionMethod::PositionNothing;

		Reg::Key(DWResizePositionMethod, static_cast<uint32_t>(posmethod));
	}

	void SetView::DoToggleResizeWindow(bool newState) {
		SetCheckBox(IDC_SIZETOIMAGE, newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_SCREEN), newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_CURRENT), newState);
		EnableWindow(GetDlgItem(Handle(), IDC_SIZETO_NOTHING), newState);
	}

	void SetView::SetupFilterBox(Win::ComboBox* ctrl) {
		ctrl->Reset(); //SendDlgItemMessage(Handle(), id, CB_RESETCONTENT, 0, 0); 
		ctrl->AddItem(SIDFilterNearestNeighbor, Filter::FilterNearestNeighbor);
		ctrl->AddItem(SIDFilterBilinear, Filter::FilterBilinear);
		ctrl->AddItem(SIDFilterLanczos3, Filter::FilterLanczos3);
	}

	SetView::SetView():App::SettingsPage(IDD_SET_VIEWER) {}
}
