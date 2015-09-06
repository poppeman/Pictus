#include "StdAfx.h"
#include "menu.h"
#include "menu_item.h"
#include "sub_menu.h"
#include "separator.h"

namespace Win {
	DWORD Menu::Display(const Win::BaseWindow* parent, const Geom::PointInt& pos, bool absoluteCoords) {
		if (!absoluteCoords)
			ClientToScreen(parent->Handle(), (LPPOINT)&pos); 

		return TrackPopupMenu(
			m_hMenu,
			TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_LEFTBUTTON,
			pos.X,
			pos.Y,
			0,
			parent->Handle(),
			0); 
	}

	void Menu::CheckMenuItem(uint32_t id, bool status) {
		m_checked.erase(id);
		m_checked.insert(std::pair<int, bool>(id, status));

		::CheckMenuItem(m_hMenu, id, (status?MF_CHECKED:MF_UNCHECKED));
	}

	void Menu::AddItem(uint32_t id, App::StringID str_id) {
		MenuPtr mi(new MenuItem(id));

		m_items.push_back(mi);

		mi->Create(str_id, m_iCurrIndex++);
		mi->AddTo(m_hMenu);
	}

	Menu::Ptr Menu::AddSubMenu(App::StringID str_id) {
		MenuPtr mi(new SubMenu());

		m_items.push_back(mi);

		mi->Create(str_id, m_iCurrIndex++);
		return mi->AddTo(m_hMenu);
	}

	void Menu::AddMenuSeparator() {
		MenuPtr mi(new Separator());

		m_items.push_back(mi);

		mi->Create(App::SIDInvalid, m_iCurrIndex++);
		mi->AddTo(m_hMenu);
	}

	void Menu::RebuildMenu() {
		DestroyMenu(m_hMenu);
		m_hMenu = CreatePopupMenu();

		for (const auto& i : m_items) {
			i->AddTo(m_hMenu);
		}

		for (const auto& i : m_checked) {
			::CheckMenuItem(m_hMenu, i.first, (i.second ? MF_CHECKED : MF_UNCHECKED));
		}
	}

	Menu::Menu():
		m_iCurrIndex{ 0 },
		m_hMenu{ 0 }
	{
		m_lang = Intl::OnLanguageChanged.connect([&]() { RebuildMenu(); });
		m_hMenu = CreatePopupMenu();
	}

	Menu::~Menu() {
		if (m_hMenu) {
			DestroyMenu(m_hMenu);
		}
	}

	HMENU Menu::Handle() {
		return m_hMenu;
	}
}
