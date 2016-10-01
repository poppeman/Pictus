#include <wx/sizer.h>
#include <wx/statbox.h>
#include "dlg_language.h"
#include "registry.h"
#include "wintypes.h"
#include "settings_layout.h"

namespace App
{
	using namespace Intl;

	class IntClientData : public wxClientData
	{
	public:
		unsigned Data;

		IntClientData(unsigned data) :
			Data(data)
		{}
	};

	std::string SetLanguage::Caption()
	{
		return Intl::GetString(SIDSettingsLanguage);
	}

	void SetLanguage::PerformUpdateFromSettings(const Reg::Settings &settings)
	{

	}


	void SetLanguage::onWriteSettings(Reg::Settings &settings)
	{
		settings.View.Language = GetSelection();
		CurrentLanguage(GetSelection());
	}

	SetLanguage::SetLanguage(wxWindow *parent) :
		SettingsPage{parent}
	{
		auto langBox = new wxStaticBoxSizer(wxVERTICAL, this, Win::GetStringWx(SIDSettingsLanguage));
		m_cbLang = new wxChoice(langBox->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize);
		langBox->Add(m_cbLang, StaticBoxInnerPadding(0));

		for (auto i = 0; i < static_cast<int>(Intl::Language::Undefined); ++i)
		{
			m_cbLang->Append(Win::GetStringWx(SIDCurrentLanguage, static_cast<Intl::Language>(i)),
			                 new IntClientData(i));
		}
		SetSelection(Intl::CurrentLanguage());

		auto topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(langBox, wxSizerFlags(0).Expand());

		SetSizerAndFit(topSizer);
	}

	void SetLanguage::SetSelection(Language lang)
	{
		for (unsigned i = 0; i < m_cbLang->GetCount(); i++)
		{
			auto clientData = dynamic_cast<IntClientData *>(m_cbLang->GetClientObject(i));
			if (clientData == nullptr)
			{
				throw EXCEPTION(Err::CriticalError, "Client data had wrong type");
			}
			if (clientData->Data == static_cast<unsigned>(lang))
			{
				m_cbLang->SetSelection(i);
				return;
			}
		}
	}

	Intl::Language SetLanguage::GetSelection()
	{
		auto clientData = dynamic_cast<IntClientData *>(m_cbLang->GetClientObject(m_cbLang->GetSelection()));
		if (clientData == nullptr)
		{
			throw EXCEPTION(Err::CriticalError, "Client data had wrong type");
		}
		return Intl::Language(clientData->Data);
	}
}
