#include "sysinfo.h"
#include <windows.h>

namespace Sys {
	namespace Info {
		size_t TotalPhysicalMemory() {
			MEMORYSTATUSEX stat;
			stat.dwLength = sizeof(stat);
			GlobalMemoryStatusEx(&stat);
			return stat.ullTotalPhys;
		}

		Intl::Language SystemLanguage() {
			switch(GetUserDefaultUILanguage()) {
				case 0x041d:
					return Intl::Language::Swedish;
				default:
					return Intl::Language::English;
			}
		}

		const std::wstring ExePath() {
			wchar_t strExe[MAX_PATH];
			::GetModuleFileNameW(0, strExe, (sizeof(strExe) / sizeof(wchar_t)) - 1);
			return std::wstring(strExe);
		}
	}
}
