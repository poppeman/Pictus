#include "res_settings.h"
#include "dlg_controls.h"
#include "registry.h"

namespace App {
	using namespace Intl;

	bool SetControls::PerformOnInitPage() {
		Caption(SIDSettingsControls);

		ControlText(IDC_GROUP_CLICK, SIDSettingsControlsClick);
		ControlText(IDC_TEXT_CTRL_LMOUSECLICK, SIDSettingsControlsLeftMouse);
		ControlText(IDC_TEXT_CTRL_MMOUSECLICK, SIDSettingsControlsMiddleMouse);
		ControlText(IDC_TEXT_CTRL_RMOUSECLICK, SIDSettingsControlsRightMouse);
		ControlText(IDC_TEXT_CTRL_XMOUSE1CLICK, SIDSettingsControlsExtra1Mouse);
		ControlText(IDC_TEXT_CTRL_XMOUSE2CLICK, SIDSettingsControlsExtra2Mouse);

		ControlText(IDC_GROUP_DOUBLECLICK, SIDSettingsControlsDoubleClick);
		ControlText(IDC_TEXT_CTRL_LMOUSEDOUBLECLICK, SIDSettingsControlsLeftMouse);
		ControlText(IDC_TEXT_CTRL_MMOUSEDOUBLECLICK, SIDSettingsControlsMiddleMouse);
		ControlText(IDC_TEXT_CTRL_RMOUSEDOUBLECLICK, SIDSettingsControlsRightMouse);
		ControlText(IDC_TEXT_CTRL_XMOUSE1DOUBLECLICK, SIDSettingsControlsExtra1Mouse);
		ControlText(IDC_TEXT_CTRL_XMOUSE2DOUBLECLICK, SIDSettingsControlsExtra2Mouse);

		ControlText(IDC_GROUP_MWHEEL, SIDSettingsControlsScrollWheel);
		ControlText(IDC_TEXT_CTRL_MWHEELUP, SIDSettingsControlsWheelUp);
		ControlText(IDC_TEXT_CTRL_MWHEELDOWN, SIDSettingsControlsWheelDown);

		m_cbLeftMouse = CreateComboBox(IDC_COMBO_CTRL_LMOUSE);
		m_cbMiddleMouse = CreateComboBox(IDC_COMBO_CTRL_MMOUSE);
		m_cbRightMouse = CreateComboBox(IDC_COMBO_CTRL_RMOUSE);
		m_cbExtra1Mouse = CreateComboBox(IDC_COMBO_CTRL_XMOUSE1);
		m_cbExtra2Mouse = CreateComboBox(IDC_COMBO_CTRL_XMOUSE2);

		m_cbLeftMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_LMOUSEDBL);
		m_cbMiddleMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_MMOUSEDBL);
		m_cbRightMouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_RMOUSEDBL);
		m_cbExtra1MouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_XMOUSE1DBL);
		m_cbExtra2MouseDoubleClick = CreateComboBox(IDC_COMBO_CTRL_XMOUSE2DBL);

		m_cbWheelDown = CreateComboBox(IDC_COMBO_CTRL_MWHEELDOWN);
		m_cbWheelUp = CreateComboBox(IDC_COMBO_CTRL_MWHEELUP);

		initMouseButtonList(m_cbLeftMouse);
		initMouseButtonList(m_cbMiddleMouse);
		initMouseButtonList(m_cbRightMouse);
		initMouseButtonList(m_cbExtra1Mouse);
		initMouseButtonList(m_cbExtra2Mouse);

		initMouseDblList(m_cbLeftMouseDoubleClick);
		initMouseDblList(m_cbMiddleMouseDoubleClick);
		initMouseDblList(m_cbRightMouseDoubleClick);
		initMouseDblList(m_cbExtra1MouseDoubleClick);
		initMouseDblList(m_cbExtra2MouseDoubleClick);

		initMouseWheelList(m_cbWheelUp);
		initMouseWheelList(m_cbWheelDown);

		return true;
	}

	void SetControls::PerformUpdateFromSettings(const Reg::Settings& settings) {
		m_cbLeftMouse->SetSelection(settings.Mouse.OnMouseLeft);
		m_cbMiddleMouse->SetSelection(settings.Mouse.OnMouseMiddle);
		m_cbRightMouse->SetSelection(settings.Mouse.OnMouseRight);
		m_cbExtra1Mouse->SetSelection(settings.Mouse.OnMouseExtra1);
		m_cbExtra2Mouse->SetSelection(settings.Mouse.OnMouseExtra2);

		m_cbLeftMouseDoubleClick->SetSelection(settings.Mouse.OnMouseLeftDbl);
		m_cbMiddleMouseDoubleClick->SetSelection(settings.Mouse.OnMouseMiddleDbl);
		m_cbRightMouseDoubleClick->SetSelection(settings.Mouse.OnMouseRightDbl);
		m_cbExtra1MouseDoubleClick->SetSelection(settings.Mouse.OnMouseExtra1Dbl);
		m_cbExtra2MouseDoubleClick->SetSelection(settings.Mouse.OnMouseExtra2Dbl);

		m_cbWheelUp->SetSelection(settings.Mouse.OnMouseWheelUp);
		m_cbWheelDown->SetSelection(settings.Mouse.OnMouseWheelDown);
	}


	void SetControls::onWriteSettings(Reg::Settings& settings) {
		settings.Mouse.OnMouseLeft = App::MouseAction(m_cbLeftMouse->GetSelectionData());
		settings.Mouse.OnMouseMiddle = App::MouseAction(m_cbMiddleMouse->GetSelectionData());
		settings.Mouse.OnMouseRight = App::MouseAction(m_cbRightMouse->GetSelectionData());
		settings.Mouse.OnMouseExtra1 = App::MouseAction(m_cbExtra1Mouse->GetSelectionData());
		settings.Mouse.OnMouseExtra2 = App::MouseAction(m_cbExtra2Mouse->GetSelectionData());

		settings.Mouse.OnMouseLeftDbl = App::MouseAction(m_cbLeftMouseDoubleClick->GetSelectionData());
		settings.Mouse.OnMouseMiddleDbl = App::MouseAction(m_cbMiddleMouseDoubleClick->GetSelectionData());
		settings.Mouse.OnMouseRightDbl = App::MouseAction(m_cbRightMouseDoubleClick->GetSelectionData());
		settings.Mouse.OnMouseExtra1Dbl = App::MouseAction(m_cbExtra1MouseDoubleClick->GetSelectionData());
		settings.Mouse.OnMouseExtra2Dbl = App::MouseAction(m_cbExtra2MouseDoubleClick->GetSelectionData());

		settings.Mouse.OnMouseWheelUp = App::MouseAction(m_cbWheelUp->GetSelectionData());
		settings.Mouse.OnMouseWheelDown = App::MouseAction(m_cbWheelDown->GetSelectionData());
	}

	SetControls::SetControls():
		App::SettingsPage(IDD_SET_CTRL_MOUSE),
		m_cbLeftMouse{ nullptr },
		m_cbMiddleMouse{ nullptr },
		m_cbRightMouse{ nullptr },
		m_cbExtra1Mouse{ nullptr },
		m_cbExtra2Mouse{ nullptr },
		m_cbLeftMouseDoubleClick{ nullptr },
		m_cbMiddleMouseDoubleClick{ nullptr },
		m_cbRightMouseDoubleClick{ nullptr },
		m_cbExtra1MouseDoubleClick{ nullptr },
		m_cbExtra2MouseDoubleClick{ nullptr },
		m_cbWheelUp{ nullptr },
		m_cbWheelDown{ nullptr }
	{}

	void SetControls::initMouseButtonList(Win::ComboBox* ctrl) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionPan, MousePan);
		ctrl->AddItem(SIDActionContextMenu, MouseContext);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);
		ctrl->AddItem(SIDActionNextImage, MouseNextImage);
		ctrl->AddItem(SIDActionPreviousImage, MousePrevImage);
		ctrl->AddItem(SIDActionZoomIn, MouseZoomIn);
		ctrl->AddItem(SIDActionZoomOut, MouseZoomOut);
	}

	void SetControls::initMouseDblList(Win::ComboBox* ctrl) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionContextMenu, MouseContext);
		ctrl->AddItem(SIDActionToggleFullSizeDefaultZoom, MouseToggleFullSizeDefaultZoom);
		ctrl->AddItem(SIDActionFullscreen, MouseFullscreen);
		ctrl->AddItem(SIDActionNextImage, MouseNextImage);
		ctrl->AddItem(SIDActionPreviousImage, MousePrevImage);
		ctrl->AddItem(SIDActionZoomIn, MouseZoomIn);
		ctrl->AddItem(SIDActionZoomOut, MouseZoomOut);
	}

	void SetControls::initMouseWheelList(Win::ComboBox* ctrl) {
		ctrl->Reset();
		ctrl->AddItem(SIDActionDisable, MouseDisable);
		ctrl->AddItem(SIDActionNextImage, MouseNextImage);
		ctrl->AddItem(SIDActionPreviousImage, MousePrevImage);
		ctrl->AddItem(SIDActionZoomIn, MouseZoomIn);
		ctrl->AddItem(SIDActionZoomOut, MouseZoomOut);
	}
}
