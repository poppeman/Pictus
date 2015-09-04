#ifndef ORZ_INTL_TABLE_ENTRY_H
#define ORZ_INTL_TABLE_ENTRY_H

#include "intl_language.h"

namespace Intl{
	struct TableEntry {
		const wchar_t* langs[static_cast<int>(Language::Undefined)];
	};
}

#endif
