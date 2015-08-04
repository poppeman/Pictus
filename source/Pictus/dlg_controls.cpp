#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_controls.h"
#include "registry.h"

namespace App {
	using namespace Intl;

	bool SetControls::PerformOnInitPage(const Reg::Settings& settings) {
		Caption(SIDControls);

		m_cbLeftMouse = CreateComboBox(IDC_COMBO_CTRL_LMOUSE);
		m_cbMiddleMouse = CreateComboBox(IDC_COMBO_CTRL_MMOUSE);
		m_cbRightMouse = CreateComboBox(IDC_COMBO_CTRL_RMOUSE);

		m_cbLeftMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_LMOUSEDBL);
		m_cbMiddleMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_MMOUSEDBL);
		m_cbRightMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_RMOUSEDBL);

		m_cbWheelDown = CreateComboBox(IDC_COMBO_CTRL_MWHEELDOWN);
		m_cbWheelUp = CreateComboBox(IDC_COMBO_CTRL_MWHEELUP);

		initMouseButtonList(m_cbLeftMouse, settings.Mouse.OnMouseLeft);
		initMouseButtonList(m_cbMiddleMouse, settings.Mouse.OnMouseMiddle);
		initMouseButtonList(m_cbRightMouse, settings.Mouse.OnMouseRight);

		initMouseDblList(m_cbLeftMouseDoubleClick, settings.Mouse.OnMouseLeftDbl);
		initMouseDblList(m_cbMiddleMouseDoubleClick, settings.Mouse.OnMouseMiddleDbl);
		initMouseDblList(m_cbRightMouseDoubleClick, settings.Mouse.OnMouseRightDbl);

		initMouseWheelList(m_cbWheelUp, settings.Mouse.OnMouseWheelUp);
		initMouseWheelList(m_cbWheelDown, settings.Mouse.OnMouseWheelDown);

		return true;
	}

	void SetControls::onWriteSettings(Reg::Settings& settings) {
		settings.Mouse.OnMouseLeft = App::MouseAction(m_cbLeftMouse->GetSelectionData());
		settings.Mouse.OnMouseMiddle = App::MouseAction(m_cbMiddleMouse->GetSelectionData());
		settings.Mouse.OnMouseRight = App::MouseAction(m_cbRightMouse->GetSelectionData());

		settings.Mouse.OnMouseLeftDbl = App::MouseAction(m_cbLeftMouseDoubleClick->GetSelectionData());
		settings.Mouse.OnMouseMiddleDbl = App::MouseAction(m_cbMiddleMouseDoubleClick->GetSelectionData());
		settings.Mouse.OnMouseRightDbl = App::MouseAction(m_cbRightMouseDoubleClick->GetSelectionData());

		settings.Mouse.OnMouseWheelUp = App::MouseAction(m_cbWheelUp->GetSelectionData());
		settings.Mouse.OnMouseWheelDown = App::MouseAction(m_cbWheelDown->GetSelectionData());
	}

	SetControls::SetControls()
		:App::SettingsPage(IDD_SET_CTRL_MOUSE)
	{}

	void SetControls::initMouseButtonList(Win::ComboBox* ctrl, App::MouseAction action) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionPan, MousePan);
		ctrl->AddItem(SIDActionContextMenu, MouseContext);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);

		ctrl->SetSelection(action);
	}

	void SetControls::initMouseDblList(Win::ComboBox* ctrl, App::MouseAction action) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);

		ctrl->SetSelection(action);
	}

	void SetControls::initMouseWheelList(Win::ComboBox* ctrl, App::MouseAction action) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionNextImage, MouseNextImage);
		ctrl->AddItem(SIDActionPrevImage, MousePrevImage);
		ctrl->AddItem(SIDActionZoomIn, MouseZoomIn);
		ctrl->AddItem(SIDActionZoomOut, MouseZoomOut);
		ctrl->SetSelection(action);
	}
}
