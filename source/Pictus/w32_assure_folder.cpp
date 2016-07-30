#include "w32_assure_folder.h"
#include "orz/exception.h"
#include "orz/fileops.h"
#include "orz/types.h"
#ifdef _WIN32
#include <windows.h>
#include <ShlObj.h>
#endif

std::string assure_folder(std::string name) {
#ifdef WIN32
	// See if there is an ini next to the .exe-file. If so, use that one (Portable mode).
	wchar_t wexePath[MAX_PATH];
	if (GetModuleFileName(GetModuleHandle(nullptr), wexePath, MAX_PATH) != 0) {
		auto exePath = WStringToUTF8(wexePath);
		auto totalExe = IO::GetPath(exePath);
		totalExe += filename;
		if (IO::DoFileExist(totalExe)) {
			return totalExe;
		}
	}

	// Always use the Application Data folder as a base
	wchar_t sLoc[MAX_PATH];
	if (FAILED(SHGetFolderPath(0, CSIDL_APPDATA, 0, 0, sLoc)))
	{
		DO_THROW(Err::CriticalError, "Could not find settings directory.");
	}

	auto currentPath(WStringToUTF8(sLoc));
	currentPath += "\\";

	// Create all sub directories one by one.
	size_t index	= 1;
	do {
		size_t newindex = path.find_first_of("\\/", index);
		if (newindex == std::string::npos) break;

		currentPath += path.substr(index, newindex - index + 1);
		CreateDirectory(UTF8ToWString(currentPath).c_str(), 0);
		index = newindex;

		if (index != std::string::npos) ++index;
	}
	while (index != std::string::npos);

	return currentPath + filename;
#else
	// TODO: Implement
	auto path = IO::GetPath(name);
	auto filename = IO::GetFile(name);
	return path + "/" + filename;
#endif
}
