#ifndef ORZ_SYSINFO_H
#define ORZ_SYSINFO_H

#include "intl.h"

namespace Sys {
	namespace Info {
		_Check_return_ size_t TotalPhysicalMemory();
		_Check_return_ Intl::Language SystemLanguage();
		_Check_return_ const std::wstring ExePath();
	}
}

#endif
