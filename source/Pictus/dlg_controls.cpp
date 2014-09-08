#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_controls.h"
#include "registry.h"

namespace App {
	using namespace Reg::Keys;
	using namespace Intl;

	bool SetControls::PerformOnInitPage() {
		Caption(SIDControls);

		m_cbLeftMouse = CreateComboBox(IDC_COMBO_CTRL_LMOUSE);
		m_cbMiddleMouse = CreateComboBox(IDC_COMBO_CTRL_MMOUSE);
		m_cbRightMouse = CreateComboBox(IDC_COMBO_CTRL_RMOUSE);

		m_cbLeftMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_LMOUSEDBL);
		m_cbMiddleMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_MMOUSEDBL);
		m_cbRightMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_RMOUSEDBL);

		m_cbWheelDown = CreateComboBox(IDC_COMBO_CTRL_MWHEELDOWN);
		m_cbWheelUp = CreateComboBox(IDC_COMBO_CTRL_MWHEELUP);

		initMouseButtonList(m_cbLeftMouse, DWOnMouseLeft);
		initMouseButtonList(m_cbMiddleMouse, DWOnMouseMiddle);
		initMouseButtonList(m_cbRightMouse, DWOnMouseRight);

		initMouseDblList(m_cbLeftMouseDoubleClick, DWOnMouseLeftDbl);
		initMouseDblList(m_cbMiddleMouseDoubleClick, DWOnMouseMiddleDbl);
		initMouseDblList(m_cbRightMouseDoubleClick, DWOnMouseRightDbl);

		initMouseWheelList(m_cbWheelUp, DWOnMouseWheelUp);
		initMouseWheelList(m_cbWheelDown, DWOnMouseWheelDown);

		return true;
	}

	void SetControls::onWriteSettings() {
		Reg::Key(DWOnMouseLeft, m_cbLeftMouse->GetSelectionData());
		Reg::Key(DWOnMouseMiddle, m_cbMiddleMouse->GetSelectionData());
		Reg::Key(DWOnMouseRight, m_cbRightMouse->GetSelectionData());

		Reg::Key(DWOnMouseLeftDbl, m_cbLeftMouseDoubleClick->GetSelectionData());
		Reg::Key(DWOnMouseMiddleDbl, m_cbMiddleMouseDoubleClick->GetSelectionData());
		Reg::Key(DWOnMouseRightDbl, m_cbRightMouseDoubleClick->GetSelectionData());

		Reg::Key(DWOnMouseWheelUp, m_cbWheelUp->GetSelectionData());
		Reg::Key(DWOnMouseWheelDown, m_cbWheelDown->GetSelectionData());
	}

	SetControls::SetControls():App::SettingsPage(IDD_SET_CTRL_MOUSE) {}

	void SetControls::initMouseButtonList(Win::ComboBox* ctrl, DWORD key, bool resetSelection) {
		DWORD currentSel = ctrl->GetSelectionData();
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionPan, MousePan);
		ctrl->AddItem(SIDActionContextMenu, MouseContext);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);

		ctrl->SetSelection(resetSelection?Reg::Key(DWORDKey(key)):currentSel);
	}

	void SetControls::initMouseDblList(Win::ComboBox* ctrl, DWORD key, bool resetSelection) {
		DWORD currentSel = ctrl->GetSelectionData();
	    ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);

		ctrl->SetSelection(resetSelection?Reg::Key(DWORDKey(key)):currentSel);
	}

	void SetControls::initMouseWheelList(Win::ComboBox* ctrl, DWORD key, bool resetSelection) {
		DWORD currentSel = ctrl->GetSelectionData();
	    ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionNextImage, MouseNextImage);
		ctrl->AddItem(SIDActionPrevImage, MousePrevImage);
		ctrl->AddItem(SIDActionZoomIn, MouseZoomIn);
		ctrl->AddItem(SIDActionZoomOut, MouseZoomOut);
		ctrl->SetSelection(resetSelection?Reg::Key(DWORDKey(key)):currentSel);
	}
}
