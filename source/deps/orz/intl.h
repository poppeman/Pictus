#ifndef ORZ_INTL_H
#define ORZ_INTL_H

#include "intl_table_entry.h"
#pragma warning(push)
#pragma warning(disable: 4512)
#include <boost/signals2.hpp>

namespace Intl {
	void LanguageTable(const TableEntry* pTable);
	void CurrentLanguage(const Language& lang);
	const Language CurrentLanguage();
	Language SystemLanguage();
	const char* GetString(int id);
	const char* GetStringLang(int id, Language lang);

	extern boost::signals2::signal<void()> OnLanguageChanged;
}

#pragma warning(pop)

#endif
