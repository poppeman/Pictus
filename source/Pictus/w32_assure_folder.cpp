#include "StdAfx.h"
#include "w32_assure_folder.h"
#include "io.h"
#include "orz/fileops.h"

std::wstring assure_folder(std::wstring name) {
	std::wstring path = IO::GetPath(name);
	std::wstring filename = IO::GetFile(name);

	// Always use the Application Data folder as a base
	wchar_t sLoc[MAX_PATH];
	COND_STRICT(SUCCEEDED(SHGetFolderPath(0, CSIDL_APPDATA, 0, 0, sLoc)), Err::CriticalError, TX("Could not find settings directory."));

	std::wstring currentPath(sLoc);
	currentPath += TX("\\");

	// Create all sub directories one by one.
	size_t index	= 1;
	do {
		size_t newindex = path.find_first_of(TX("\\/"), index);
		if (newindex == std::wstring::npos) break;

		currentPath += path.substr(index, newindex - index + 1);
		CreateDirectory(currentPath.c_str(), 0);
		index = newindex;

		if (index != std::wstring::npos) ++index;
	}
	while (index != std::wstring::npos);

	return currentPath + filename;
}
