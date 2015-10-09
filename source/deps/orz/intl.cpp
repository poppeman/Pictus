#include "intl.h"
#include "exception.h"
#include "types.h"
#include "sysinfo.h"

namespace Intl {
	boost::signals2::signal<void()> OnLanguageChanged;

	auto m_currLang = Language::English;

	// TODO: Replace this garbage
	const TableEntry* m_pTable = 0;

	void LanguageTable(const TableEntry* pTable) {
		m_pTable	= pTable;
	}

	void CurrentLanguage(const Language& lang) {
		if (m_currLang != lang) {
			m_currLang = lang;
			OnLanguageChanged();
		}
	}

	const Language CurrentLanguage() {
		return m_currLang;
	}

	Language SystemLanguage() {
		return Sys::Info::SystemLanguage();
	}

	const wchar_t* GetWString(int id) {
		return GetWStringLang(id, m_currLang);
	}

	const wchar_t* GetWStringLang(int id, Language lang) {
		if (lang >= Language::Undefined) {
			DO_THROW(Err::CriticalError, "Invalid language identifier requested.");
		}

		return m_pTable[id].langs[static_cast<size_t>(lang)];
	}
}
