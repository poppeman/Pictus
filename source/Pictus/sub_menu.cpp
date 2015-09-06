#include "StdAfx.h"
#include "sub_menu.h"

namespace Win {
	Menu::Ptr SubMenu::AddTo(HMENU hMenu) {
		if (m_pMenu) {
			m_pMenu->RebuildMenu();
		}
		else {
			m_pMenu.reset(new Menu());
		}

		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING | MIIM_SUBMENU;

		// Const pointer can't be used...
		::std::wstring str = String();
		mii.dwTypeData = (wchar_t*)str.c_str();

		mii.hSubMenu = m_pMenu->Handle();			// Attach a new menu as a child.

		InsertMenuItem(hMenu, Index(), true, &mii);

		return m_pMenu;
	}
}
