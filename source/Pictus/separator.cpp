#include "StdAfx.h"
#include "separator.h"

namespace Win {
	Menu::Ptr Separator::AddTo(HMENU hMenu) {
		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_FTYPE;		// No string is used for separators.
		mii.fType = MFT_SEPARATOR;	// Should be a separator.

		InsertMenuItem(hMenu, Index(), true, &mii);
		return Menu::Ptr();
	}

}
