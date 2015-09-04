#include "StdAfx.h"
#include "intl.h"
#include "sysinfo.h"

#pragma warning(disable: 4503)

namespace Intl {
	boost::signals2::signal<void()> OnLanguageChanged;

	auto m_currLang = Language::English;

	// TODO: Replace this garbage
	const TableEntry* m_pTable = 0;

	void LanguageTable(_In_ const TableEntry* pTable) {
		m_pTable	= pTable;
	}

	void CurrentLanguage(_In_ const Language& lang) {
		if (m_currLang != lang) {
			m_currLang = lang;
			OnLanguageChanged();
		}
	}

	_Check_return_ const Language CurrentLanguage() {
		return m_currLang;
	}

	_Check_return_ Language SystemLanguage() {
		return Sys::Info::SystemLanguage();
	}

	_Ret_z_ PCWSTR GetWString(_In_ int id) {
		return GetWStringLang(id, m_currLang);
	}

	_Ret_z_ PCWSTR GetWStringLang(_In_ int id, _In_ Language lang) {
		if (lang >= Language::Undefined) {
			DO_THROW(Err::CriticalError, TX("Invalid language identifier requested."));
		}

		return m_pTable[id].langs[static_cast<size_t>(lang)];
	}
}
