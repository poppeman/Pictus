#include "dlg_cache.h"
#include "registry.h"
#include "wintypes.h"
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/valnum.h>

namespace App
{
	using namespace Intl;

	BEGIN_EVENT_TABLE (SetPageCache, wxPanel)
		EVT_CHECKBOX(AutoLimitId, SetPageCache::OnAutoLimitChanged)
	END_EVENT_TABLE()


	bool SetPageCache::IsRootPage() const
	{
		return false;
	}

	std::string SetPageCache::Caption()
	{
		return Intl::GetString(SIDSettingsCache);
	}

	SetPageCache::SetPageCache(wxWindow* parent) :
		App::SettingsPage{ parent }
	{
		auto cacheBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsCache));

		m_autoLimit = new wxCheckBox(cacheBox->GetStaticBox(), AutoLimitId, Win::GetStringWx(SIDSettingsCacheAutomaticLimit));

		cacheBox->Add(m_autoLimit, wxSizerFlags(0).Expand());

		auto validator = wxIntegerValidator<unsigned int>();
		m_cacheSize = new wxTextCtrl(cacheBox->GetStaticBox(), CacheSizeId, "", wxDefaultPosition, wxDefaultSize, 0, validator);
		auto sizeSizer = new wxBoxSizer(wxHORIZONTAL);
		sizeSizer->Add(m_cacheSize, wxSizerFlags(0));
		sizeSizer->Add(new wxStaticText(cacheBox->GetStaticBox(), wxID_ANY, Win::GetStringWx(SIDUnitMB)), wxSizerFlags(0));
		cacheBox->Add(sizeSizer, wxSizerFlags(0));

		SetSizerAndFit(cacheBox);
		UpdateControls();
	}

	void SetPageCache::onWriteSettings(Reg::Settings &settings)
	{
		settings.Cache.DoAutoMemoryLimit = m_autoLimit->GetValue();
		settings.Cache.ManualMemoryLimit = FromAString<size_t>(ToAString(m_cacheSize->GetValue().c_str()));
	}

	void SetPageCache::UpdateControls()
	{
		m_cacheSize->Enable(!m_autoLimit->GetValue());
	}

	void SetPageCache::PerformUpdateFromSettings(const Reg::Settings &settings)
	{
		m_cacheSize->SetValue(ToAString(settings.Cache.ManualMemoryLimit));
		m_autoLimit->SetValue(settings.Cache.DoAutoMemoryLimit);
		UpdateControls();
	}

	void SetPageCache::OnAutoLimitChanged(wxCommandEvent &event)
	{
		UpdateControls();
	}
}
