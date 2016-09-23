#include "filemanager.h"
#include "orz/types.h"

#ifdef _WIN32
#include <boost/scoped_array.hpp>
#include <windows.h>
#endif

namespace App
{
	void ShowInFileManager(std::string path)
	{
#ifdef _WIN32
		// Construct command line string
		std::wstring cmd(L"explorer /e,/select,\"" + UTF8ToWString(path) + L"\"");

		// Start the new process
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Microsoft forgot "const". Copy the string to avoid casts
		boost::scoped_array<wchar_t> tmpStr(new wchar_t[cmd.length() + 1]);
		tmpStr[cmd.length()] = 0;
		wcscpy_s(tmpStr.get(), (cmd.length() + 1), cmd.c_str());

		// Finally, launch explorer.exe with the nifty params
		CreateProcess(0, tmpStr.get(), 0, 0, false, 0, 0, 0, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
#else
		wxMessageBox(L"Not yet implemented! ", L"Pictus Error", wxOK);
#endif
	}
}
