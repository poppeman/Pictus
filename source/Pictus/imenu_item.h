#ifndef PICTUS_IMENU_ITEM_H
#define PICTUS_IMENU_ITEM_H

#include "menu.h"

namespace Win {
	class Menu::IMenuItem {
	public:
		virtual Menu::Ptr AddTo(HMENU hMenu) = 0;
		void Create(App::StringID string_id, int index);

		IMenuItem();
		virtual ~IMenuItem() {}

	protected:
		std::wstring String();
		int Index();

	private:
		App::StringID m_string_id;
		int m_index;
	};
}

#endif
