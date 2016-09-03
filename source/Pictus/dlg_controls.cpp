#include "registry.h"
#include "dlg_controls.h"
#include "wintypes.h"
#include <wx/statbox.h>
#include <wx/stattext.h>

namespace App
{
	using namespace Intl;

	std::string App::SetControls::Caption()
	{
		return Intl::GetString(SIDSettingsControls);
	}

	class MouseActionClientData:public wxClientData
	{
	public:
		MouseAction Action;

		MouseActionClientData(MouseAction action):Action(action) {}
	};

	SetControls::SetControls(wxWindow *parent) :
		App::SettingsPage{parent},
		m_cbLeftMouse{nullptr},
		m_cbMiddleMouse{nullptr},
		m_cbRightMouse{nullptr},
		m_cbExtra1Mouse{nullptr},
		m_cbExtra2Mouse{nullptr},
		m_cbLeftMouseDoubleClick{nullptr},
		m_cbMiddleMouseDoubleClick{nullptr},
		m_cbRightMouseDoubleClick{nullptr},
		m_cbExtra1MouseDoubleClick{nullptr},
		m_cbExtra2MouseDoubleClick{nullptr},
		m_cbWheelUp{nullptr},
		m_cbWheelDown{nullptr}
	{
		auto clickGroup = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsControlsClick));
		m_cbLeftMouse = initMouseButtonList(clickGroup, SIDSettingsControlsLeftMouse);
		m_cbMiddleMouse = initMouseButtonList(clickGroup, SIDSettingsControlsMiddleMouse);
		m_cbRightMouse = initMouseButtonList(clickGroup, SIDSettingsControlsRightMouse);
		m_cbExtra1Mouse = initMouseButtonList(clickGroup, SIDSettingsControlsExtra1Mouse);
		m_cbExtra2Mouse = initMouseButtonList(clickGroup, SIDSettingsControlsExtra2Mouse);

		auto dblClickGroup = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsControlsDoubleClick));
		m_cbLeftMouseDoubleClick = initMouseDblList(dblClickGroup, SIDSettingsControlsLeftMouse);
		m_cbMiddleMouseDoubleClick = initMouseDblList(dblClickGroup, SIDSettingsControlsMiddleMouse);
		m_cbRightMouseDoubleClick = initMouseDblList(dblClickGroup, SIDSettingsControlsRightMouse);
		m_cbExtra1MouseDoubleClick = initMouseDblList(dblClickGroup, SIDSettingsControlsExtra1Mouse);
		m_cbExtra2MouseDoubleClick = initMouseDblList(dblClickGroup, SIDSettingsControlsExtra2Mouse);

		auto mouseWheelGroup = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsControlsScrollWheel));
		m_cbWheelUp = initMouseWheelList(mouseWheelGroup, SIDSettingsControlsWheelUp);
		m_cbWheelDown = initMouseWheelList(mouseWheelGroup, SIDSettingsControlsWheelDown);

		auto topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(clickGroup, wxSizerFlags(0));
		topSizer->Add(dblClickGroup, wxSizerFlags(0));
		topSizer->Add(mouseWheelGroup, wxSizerFlags(0));

		SetSizerAndFit(topSizer);
	}

	void SetControls::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		SetComboAction(m_cbLeftMouse, settings.Mouse.OnMouseLeft);
		SetComboAction(m_cbMiddleMouse, settings.Mouse.OnMouseMiddle);
		SetComboAction(m_cbRightMouse, settings.Mouse.OnMouseRight);
		SetComboAction(m_cbExtra1Mouse, settings.Mouse.OnMouseExtra1);
		SetComboAction(m_cbExtra2Mouse, settings.Mouse.OnMouseExtra2);

		SetComboAction(m_cbLeftMouseDoubleClick, settings.Mouse.OnMouseLeftDbl);
		SetComboAction(m_cbMiddleMouseDoubleClick, settings.Mouse.OnMouseMiddleDbl);
		SetComboAction(m_cbRightMouseDoubleClick, settings.Mouse.OnMouseRightDbl);
		SetComboAction(m_cbExtra1MouseDoubleClick, settings.Mouse.OnMouseExtra1Dbl);
		SetComboAction(m_cbExtra2MouseDoubleClick, settings.Mouse.OnMouseExtra2Dbl);

		SetComboAction(m_cbWheelUp, settings.Mouse.OnMouseWheelUp);
		SetComboAction(m_cbWheelDown, settings.Mouse.OnMouseWheelDown);
	}


	void SetControls::onWriteSettings(Reg::Settings &settings)
	{
		settings.Mouse.OnMouseLeft = GetComboAction(m_cbLeftMouse);
		settings.Mouse.OnMouseMiddle = GetComboAction(m_cbMiddleMouse);
		settings.Mouse.OnMouseRight = GetComboAction(m_cbRightMouse);
		settings.Mouse.OnMouseExtra1 = GetComboAction(m_cbExtra1Mouse);
		settings.Mouse.OnMouseExtra2 = GetComboAction(m_cbExtra2Mouse);

		settings.Mouse.OnMouseLeftDbl = GetComboAction(m_cbLeftMouseDoubleClick);
		settings.Mouse.OnMouseMiddleDbl = GetComboAction(m_cbMiddleMouseDoubleClick);
		settings.Mouse.OnMouseRightDbl = GetComboAction(m_cbRightMouseDoubleClick);
		settings.Mouse.OnMouseExtra1Dbl = GetComboAction(m_cbExtra1MouseDoubleClick);
		settings.Mouse.OnMouseExtra2Dbl = GetComboAction(m_cbExtra2MouseDoubleClick);

		settings.Mouse.OnMouseWheelUp = GetComboAction(m_cbWheelUp);
		settings.Mouse.OnMouseWheelDown = GetComboAction(m_cbWheelDown);
	}

	App::MouseAction SetControls::GetComboAction(wxChoice *cb)
	{
		return dynamic_cast<MouseActionClientData*>(cb->GetClientObject(cb->GetSelection()))->Action;
	}

	void SetControls::SetComboAction(wxChoice *cb, App::MouseAction action)
	{
		for(unsigned i=0; i < cb->GetCount(); i++)
		{
			if(dynamic_cast<MouseActionClientData*>(cb->GetClientObject(i))->Action == action)
			{
				cb->SetSelection(i);
				return;
			}
		}
	}

	wxChoice* SetControls::initMouseButtonList(wxStaticBoxSizer* parent, StringID label)
	{
		auto ctrl = new wxChoice(parent->GetStaticBox(), wxID_ANY);
		ctrl->Append(Win::GetStringWx(SIDActionDisable), new MouseActionClientData(MouseDisable));
		ctrl->Append(Win::GetStringWx(SIDActionPan), new MouseActionClientData(MousePan));
		ctrl->Append(Win::GetStringWx(SIDActionContextMenu), new MouseActionClientData(MouseContext));
		ctrl->Append(Win::GetStringWx(SIDActionToggleFullSizeDefaultZoom), new MouseActionClientData(MouseToggleFullSizeDefaultZoom));
		ctrl->Append(Win::GetStringWx(SIDActionFullscreen), new MouseActionClientData(MouseFullscreen));
		ctrl->Append(Win::GetStringWx(SIDActionNextImage), new MouseActionClientData(MouseNextImage));
		ctrl->Append(Win::GetStringWx(SIDActionPreviousImage), new MouseActionClientData(MousePrevImage));
		ctrl->Append(Win::GetStringWx(SIDActionZoomIn), new MouseActionClientData(MouseZoomIn));
		ctrl->Append(Win::GetStringWx(SIDActionZoomOut), new MouseActionClientData(MouseZoomOut));
		ctrl->Append(Win::GetStringWx(SIDActionRotateLeft), new MouseActionClientData(MouseRotateLeft));
		ctrl->Append(Win::GetStringWx(SIDActionRotateRight), new MouseActionClientData(MouseRotateRight));

		auto row = new wxBoxSizer(wxHORIZONTAL);
		row->Add(ctrl, wxSizerFlags(0));
		row->Add(new wxStaticText(parent->GetStaticBox(), wxID_ANY, Win::GetStringWx(label)), wxSizerFlags(0));

		parent->Add(row, wxSizerFlags(0));
		return ctrl;
	}

	wxChoice* SetControls::initMouseDblList(wxStaticBoxSizer* parent, StringID label)
	{
		auto ctrl = new wxChoice(parent->GetStaticBox(), wxID_ANY);
		ctrl->Append(Win::GetStringWx(SIDActionDisable), new MouseActionClientData(MouseDisable));
		ctrl->Append(Win::GetStringWx(SIDActionContextMenu), new MouseActionClientData(MouseContext));
		ctrl->Append(Win::GetStringWx(SIDActionToggleFullSizeDefaultZoom), new MouseActionClientData(MouseToggleFullSizeDefaultZoom));
		ctrl->Append(Win::GetStringWx(SIDActionFullscreen), new MouseActionClientData(MouseFullscreen));
		ctrl->Append(Win::GetStringWx(SIDActionNextImage), new MouseActionClientData(MouseNextImage));
		ctrl->Append(Win::GetStringWx(SIDActionPreviousImage), new MouseActionClientData(MousePrevImage));
		ctrl->Append(Win::GetStringWx(SIDActionZoomIn), new MouseActionClientData(MouseZoomIn));
		ctrl->Append(Win::GetStringWx(SIDActionZoomOut), new MouseActionClientData(MouseZoomOut));
		ctrl->Append(Win::GetStringWx(SIDActionRotateLeft), new MouseActionClientData(MouseRotateLeft));
		ctrl->Append(Win::GetStringWx(SIDActionRotateRight), new MouseActionClientData(MouseRotateRight));

		auto row = new wxBoxSizer(wxHORIZONTAL);
		row->Add(ctrl, wxSizerFlags(0));
		row->Add(new wxStaticText(parent->GetStaticBox(), wxID_ANY, Win::GetStringWx(label)), wxSizerFlags(0));

		parent->Add(row, wxSizerFlags(0));
		return ctrl;
	}

	wxChoice* SetControls::initMouseWheelList(wxStaticBoxSizer* parent, StringID label)
	{
		auto ctrl = new wxChoice(parent->GetStaticBox(), wxID_ANY);
		ctrl->Append(Win::GetStringWx(SIDActionDisable), new MouseActionClientData(MouseDisable));
		ctrl->Append(Win::GetStringWx(SIDActionNextImage), new MouseActionClientData(MouseNextImage));
		ctrl->Append(Win::GetStringWx(SIDActionPreviousImage), new MouseActionClientData(MousePrevImage));
		ctrl->Append(Win::GetStringWx(SIDActionZoomIn), new MouseActionClientData(MouseZoomIn));
		ctrl->Append(Win::GetStringWx(SIDActionZoomOut), new MouseActionClientData(MouseZoomOut));
		ctrl->Append(Win::GetStringWx(SIDActionRotateLeft), new MouseActionClientData(MouseRotateLeft));
		ctrl->Append(Win::GetStringWx(SIDActionRotateRight), new MouseActionClientData(MouseRotateRight));

		auto row = new wxBoxSizer(wxHORIZONTAL);
		row->Add(ctrl, wxSizerFlags(0));
		row->Add(new wxStaticText(parent->GetStaticBox(), wxID_ANY, Win::GetStringWx(label)), wxSizerFlags(0));

		parent->Add(row, wxSizerFlags(0));
		return ctrl;
	}
}
