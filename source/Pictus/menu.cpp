
#include "StdAfx.h"
#include "menu.h"

namespace Win {
	class Menu::IMenuItem {
	public:
		virtual Menu::Ptr AddTo(HMENU hMenu) =0;
		void Create(App::StringID string_id, int index) {
			m_string_id	= string_id;
			m_index		= index;
		}

		IMenuItem():m_string_id(App::SIDInvalid), m_index(0) {}
		virtual ~IMenuItem() {}

	protected:
		const wchar_t* String() {
			return Intl::GetWString(m_string_id);
		}

		int Index() {
			return m_index;
		}

	private:
		App::StringID m_string_id;
		int m_index;
	};

	class MenuItem:public Menu::IMenuItem {
	public:
		Menu::Ptr AddTo(HMENU hMenu) {
			MENUITEMINFO mii;

			ZeroMemory(&mii, sizeof(mii));
			mii.cbSize	= sizeof(mii);
			mii.fMask	= MIIM_STRING | MIIM_ID;
			mii.wID		= m_id;

			// Const pointer can't be used...
			::std::wstring str = String();
			mii.dwTypeData = const_cast<wchar_t*>(str.c_str());

			InsertMenuItem(hMenu, Index(), true, &mii);
			return Menu::Ptr();
		}

		MenuItem(DWORD id):m_id(id) {}

	private:
		DWORD m_id;
	};

	class SubMenu:public Menu::IMenuItem {
	public:
		Menu::Ptr AddTo(HMENU hMenu) {
			if (m_pMenu.get())
				m_pMenu->RebuildMenu();
			else
				m_pMenu.reset(new Menu());

			MENUITEMINFO mii;
			ZeroMemory(&mii, sizeof(mii));
			mii.cbSize	= sizeof(mii);
			mii.fMask	= MIIM_STRING | MIIM_SUBMENU;

			// Const pointer can't be used...
			::std::wstring str = String();
			mii.dwTypeData = (wchar_t*)str.c_str();

			mii.hSubMenu=m_pMenu->Handle();			// Attach a new menu as a child.

			InsertMenuItem(hMenu, Index(), true, &mii);

			return m_pMenu;
		}

	private:
		Menu::Ptr					m_pMenu;
	};

	class Separator:public Menu::IMenuItem {
	public:
		Menu::Ptr AddTo(HMENU hMenu) {
			MENUITEMINFO mii;
			ZeroMemory(&mii, sizeof(mii));
			mii.cbSize=sizeof(mii);
			mii.fMask=MIIM_FTYPE;		// No string is used for separators.
			mii.fType=MFT_SEPARATOR;	// Should be a separator.

			InsertMenuItem(hMenu, Index(), true, &mii);
			return Menu::Ptr();
		}
	};

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

		mi->Create(App::SIDUndefined, m_iCurrIndex++);
		mi->AddTo(m_hMenu);
	}

	void Menu::RebuildMenu() {
		DestroyMenu(m_hMenu);
		m_hMenu = CreatePopupMenu();

		for (const auto& i : m_items)
			i->AddTo(m_hMenu);

		for (const auto& i : m_checked)
			::CheckMenuItem(m_hMenu, i.first, (i.second ? MF_CHECKED : MF_UNCHECKED));
	}

	Menu::Menu()
		:m_iCurrIndex(0),
		m_hMenu(0)
	{
		m_lang = Intl::OnLanguageChanged.connect([&]() { RebuildMenu(); });
		m_hMenu = CreatePopupMenu();
	}

	Menu::~Menu() {
		if (m_hMenu)
			DestroyMenu(m_hMenu);
	}

	HMENU Menu::Handle() {
		return m_hMenu;
	}
}
