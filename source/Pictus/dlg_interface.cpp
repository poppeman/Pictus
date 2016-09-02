#include <wx/sizer.h>
#include <wx/statbox.h>
#include "dlg_interface.h"
#include "registry.h"
#include "wintypes.h"

namespace App
{
	bool SetInterface::IsRootPage() const
	{
		return false;
	}

	std::string SetInterface::Caption()
	{
		return Intl::GetString(SIDSettingsInterface);
	}

	SetInterface::SetInterface(wxWindow *parent) :
		App::SettingsPage{parent},
		m_showStatusBar{nullptr},
		m_alwaysOnTop{nullptr}
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);
		auto statusBox = new wxStaticBoxSizer(wxHORIZONTAL, this, Win::GetStringWx(SIDSettingsInterfaceStatusBar));
		m_showStatusBar = new wxCheckBox(statusBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDSettingsInterfaceShowStatusBar));
		statusBox->Add(m_showStatusBar, wxSizerFlags(0).Expand());
		m_alwaysOnTop = new wxCheckBox(this, wxID_ANY, Win::GetStringWx(SIDSettingsInterfaceAlwaysOnTop));

		topSizer->Add(statusBox, wxSizerFlags(0).Expand());
		topSizer->Add(m_alwaysOnTop, wxSizerFlags(0).Expand());
		SetSizerAndFit(topSizer);
	}

	void SetInterface::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_showStatusBar->SetValue(settings.View.ShowStatusBar);
		m_alwaysOnTop->SetValue(settings.View.AlwaysOnTop);
	}

	void SetInterface::onWriteSettings(Reg::Settings &settings)
	{
		settings.View.ShowStatusBar = m_showStatusBar->GetValue();
		settings.View.AlwaysOnTop = m_alwaysOnTop->GetValue();
	}
}
