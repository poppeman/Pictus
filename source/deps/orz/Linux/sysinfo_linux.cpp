#include "../sysinfo.h"
#include "../types.h"
#include "../exception.h"
#include <linux/limits.h>

namespace Sys {
	namespace Info {
		uint64_t TotalPhysicalMemory()
		{
			// TODO: Implement
			return static_cast<uint64_t>(8) * 1024 * 1024 * 1024;
		}

		Intl::Language SystemLanguage()
		{
			// TODO: Implement
			return Intl::Language::English;
		}

		const std::string ExePath()
		{
			// For fuck sake, another os with a hard-coded path length limit?
			char dest[PATH_MAX] = {0};
			auto written = readlink("/proc/self/exe", dest, PATH_MAX);
			if (written == -1)
			{
				throw EXCEPTION(std::runtime_error, "readlink on /proc/self/exec failed");
			}
			return std::string(dest, written);
		}
	}
}
