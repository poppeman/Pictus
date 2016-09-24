#include "dlg_advanced.h"
#include "registry.h"
#include "wintypes.h"

namespace App
{
	using namespace Intl;

	std::string SetAdvanced::Caption()
	{
		return Intl::GetString(SIDSettingsAdvanced);
	}

	SetAdvanced::SetAdvanced(wxWindow *parent) :
		App::SettingsPage{parent}
	{
		m_allowMultipleInstances = new wxCheckBox(this, wxID_ANY, Win::GetStringWx(SIDSettingsAdvancedAllowMultipleInstances));
	}

	void SetAdvanced::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_allowMultipleInstances->SetValue(settings.View.MultipleInstances);
	}

	void SetAdvanced::onWriteSettings(Reg::Settings &settings)
	{
		settings.View.MultipleInstances = m_allowMultipleInstances->GetValue();
	}
}