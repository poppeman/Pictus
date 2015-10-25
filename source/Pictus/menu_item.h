#ifndef PICTUS_MENU_ITEM_H
#define PICTUS_MENU_ITEM_H

#include "imenu_item.h"

namespace Win {
	class MenuItem :public Menu::IMenuItem {
	public:
		Menu::Ptr AddTo(HMENU hMenu) override;
		uint32_t Id() const;

		MenuItem(DWORD id);

	private:
		DWORD m_id;
	};
}

#endif
