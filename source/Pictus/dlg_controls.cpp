#include "StdAfx.h"
#include "res_settings.h"
#include "dlg_controls.h"
#include "registry.h"

namespace App {
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

		initMouseButtonList(m_cbLeftMouse);
		initMouseButtonList(m_cbMiddleMouse);
		initMouseButtonList(m_cbRightMouse);

		initMouseDblList(m_cbLeftMouseDoubleClick);
		initMouseDblList(m_cbMiddleMouseDoubleClick);
		initMouseDblList(m_cbRightMouseDoubleClick);

		initMouseWheelList(m_cbWheelUp);
		initMouseWheelList(m_cbWheelDown);

		return true;
	}

	void SetControls::PerformUpdateFromSettings(const Reg::Settings& settings) {
		m_cbLeftMouse->SetSelection(settings.Mouse.OnMouseLeft);
		m_cbMiddleMouse->SetSelection(settings.Mouse.OnMouseMiddle);
		m_cbRightMouse->SetSelection(settings.Mouse.OnMouseRight);
		m_cbLeftMouseDoubleClick->SetSelection(settings.Mouse.OnMouseLeftDbl);
		m_cbMiddleMouseDoubleClick->SetSelection(settings.Mouse.OnMouseMiddleDbl);
		m_cbRightMouseDoubleClick->SetSelection(settings.Mouse.OnMouseRightDbl);
		m_cbWheelUp->SetSelection(settings.Mouse.OnMouseWheelUp);
		m_cbWheelDown->SetSelection(settings.Mouse.OnMouseWheelDown);
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

	void SetControls::initMouseButtonList(Win::ComboBox* ctrl) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionPan, MousePan);
		ctrl->AddItem(SIDActionContextMenu, MouseContext);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);
	}

	void SetControls::initMouseDblList(Win::ComboBox* ctrl) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);
	}

	void SetControls::initMouseWheelList(Win::ComboBox* ctrl) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionNextImage, MouseNextImage);
		ctrl->AddItem(SIDActionPrevImage, MousePrevImage);
		ctrl->AddItem(SIDActionZoomIn, MouseZoomIn);
		ctrl->AddItem(SIDActionZoomOut, MouseZoomOut);
	}
}
