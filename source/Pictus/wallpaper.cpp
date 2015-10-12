#include "wallpaper.h"

#include "orz/types.h"

#include <windows.h>
#include <WinInet.h>
#include <ShlObj.h>

namespace Win {
	namespace Wallpaper {
		bool ApplyWallpaper(const std::string& file, Mode m) {
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
			case Mode::Stretch:
				wpOpt.dwStyle = WPSTYLE_STRETCH;
				break;
			case Mode::Center:
				wpOpt.dwStyle = WPSTYLE_CENTER;
				break;
			case Mode::Tile:
				wpOpt.dwStyle = WPSTYLE_TILE;
				break;
			case Mode::FillPad:
				wpOpt.dwStyle = WPSTYLE_KEEPASPECT;
				break;
			case Mode::FillCrop:
				wpOpt.dwStyle = WPSTYLE_CROPTOFIT;
				break;
			case Mode::Span:
				wpOpt.dwStyle = WPSTYLE_SPAN;  // MSDN claims that WPSTYLE_MAX must also be set, but that seems to be incorrect.
				break;
			}

			pActiveDesktop->SetWallpaperOptions(&wpOpt, 0);
			pActiveDesktop->SetWallpaper(UTF8ToWString(file).c_str(), 0);
			pActiveDesktop->ApplyChanges(AD_APPLY_ALL);

			pActiveDesktop->Release();
			return true;
		}
	}
}
