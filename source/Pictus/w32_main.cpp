#include "app_types.h"
#include "main.h"
#include "wintypes.h"
#include "orz/fileops.h"
#include "orz/sysinfo.h"

#include <Uxtheme.h>
#include <objbase.h>

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ wchar_t* pCmdLine, _In_ int) {
	SetThemeAppProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS | STAP_ALLOW_WEBCONTENT);

	INITCOMMONCONTROLSEX icce;
	icce.dwSize = sizeof(icce);
	icce.dwICC = ICC_BAR_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS | ICC_LISTVIEW_CLASSES;
	if (InitCommonControlsEx(&icce) == FALSE) {
		::MessageBox(0, TX("Couldn't initialize common controls."), 0, MB_OK);
		return EXIT_FAILURE;
	}

	HRESULT hret = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hret)) {
		::MessageBox(0, TX("CoInitializeEx failed.\nThis is bad."), 0, MB_OK);
		return EXIT_FAILURE;
	}

	SetCurrentDirectory(IO::GetPath(Sys::Info::ExePath()).c_str());

	int ret = start_app(Win::LongPath(pCmdLine));

	return ret;
}
