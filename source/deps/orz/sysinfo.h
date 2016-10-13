#ifndef ORZ_SYSINFO_H
#define ORZ_SYSINFO_H

#include "intl.h"

namespace Sys {
	namespace Info {
		uint64_t TotalPhysicalMemory();
		Intl::Language SystemLanguage();
		const std::string ExePath();

		std::string InvalidFilenameCharacters();
	}
}

#endif
