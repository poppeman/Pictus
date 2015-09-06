#ifndef PICTUS_SUB_MENU_H
#define PICTUS_SUB_MENU_H

#include "imenu_item.h"

namespace Win {
	class SubMenu :public Menu::IMenuItem {
	public:
		Menu::Ptr AddTo(HMENU hMenu) override;

	private:
		Menu::Ptr m_pMenu;
	};
}


#endif
