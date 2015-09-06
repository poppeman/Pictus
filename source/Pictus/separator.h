#ifndef PICTUS_SEPARATOR_H
#define PICTUS_SEPARATOR_H

#include "imenu_item.h"

namespace Win {
	class Separator :public Menu::IMenuItem {
	public:
		Menu::Ptr AddTo(HMENU hMenu) override;
	};
}

#endif
