#include "menu_item.h"

namespace Win {
	Menu::Ptr MenuItem::AddTo(HMENU hMenu) {
		MENUITEMINFO mii;

		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING | MIIM_ID;
		mii.wID = m_id;

		// Const pointer can't be used...
		::std::wstring str = String();
		mii.dwTypeData = const_cast<wchar_t*>(str.c_str());

		InsertMenuItem(hMenu, Index(), true, &mii);
		return Menu::Ptr();
	}

	MenuItem::MenuItem(DWORD id):
		m_id{ id }
	{}
}
