#ifndef ORZ_INTL_H
#define ORZ_INTL_H

#include "intl_table_entry.h"
#include <boost/signals2.hpp>

namespace Intl {
	void LanguageTable(const TableEntry* pTable);
	void CurrentLanguage(const Language& lang);
	const Language CurrentLanguage();
	Language SystemLanguage();
	const wchar_t* GetWString(int id);
	const wchar_t* GetWStringLang(int id, Language lang);

	extern boost::signals2::signal<void()> OnLanguageChanged;
}

#endif
