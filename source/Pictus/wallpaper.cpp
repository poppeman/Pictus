#include "StdAfx.h"
#include "wallpaper.h"

namespace Win {
	bool Wallpaper::ApplyWallpaper(const std::wstring& file, Mode m) {
		IActiveDesktop *pActiveDesktop;
		if (CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&pActiveDesktop) != S_OK)
			return false;

		if (pActiveDesktop == 0)
			return false;

		WALLPAPEROPT wpOpt;
		COMPONENTSOPT cOpt;

		cOpt.dwSize = sizeof(COMPONENTSOPT);
		cOpt.fActiveDesktop = true;
		cOpt.fEnableComponents = true;
		pActiveDesktop->SetDesktopItemOptions(&cOpt, 0);

		wpOpt.dwSize = sizeof(WALLPAPEROPT);
		switch (m) {
		case WPStretch:
			wpOpt.dwStyle = WPSTYLE_STRETCH;
			break;
		case WPCenter:
			wpOpt.dwStyle = WPSTYLE_CENTER;
			break;
		case WPTile:
			wpOpt.dwStyle = WPSTYLE_TILE;
			break;
		}

		pActiveDesktop->SetWallpaperOptions(&wpOpt, 0);
		pActiveDesktop->SetWallpaper(file.c_str(), 0);
		pActiveDesktop->ApplyChanges(AD_APPLY_ALL);

		pActiveDesktop->Release();
		return true;
	}
}
