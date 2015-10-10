#ifndef ORZ_SYSINFO_H
#define ORZ_SYSINFO_H

#include "intl.h"

namespace Sys {
	namespace Info {
		size_t TotalPhysicalMemory();
		Intl::Language SystemLanguage();
		const std::wstring ExePath();
	}
}

#endif
