#include "w32_assure_folder.h"
#include "orz/exception.h"
#include "orz/fileops.h"
#include "orz/types.h"

#include <wx/stdpaths.h>
#include <boost/filesystem.hpp>

std::string assure_folder(std::string name) {
	// See if there is an ini next to the .exe-file. If so, use that one (Portable mode).
	wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_AppName | wxStandardPaths::AppInfo_VendorName);
	auto currentExe = std::string(wxStandardPaths::Get().GetExecutablePath().ToUTF8());
	auto exePath = boost::filesystem::path(currentExe).parent_path();
	auto totalExe = ToAString(exePath / name);
	if(IO::DoFileExist(totalExe))
	{
		return totalExe;
	}

	// Always use the Application Data folder as a base
	auto configDir = std::string(wxStandardPaths::Get().GetUserDataDir().ToUTF8());
	if(configDir.size() == 0)
	{
		DO_THROW(Err::CriticalError, "Could not find settings directory.");
	}

	auto configPath = boost::filesystem::path(configDir);
	boost::filesystem::create_directories(configPath);
	auto totalConfig = configPath / name;

	return ToAString(totalConfig).c_str();
}
