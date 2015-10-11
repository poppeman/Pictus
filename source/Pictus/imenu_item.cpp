#include "imenu_item.h"

namespace Win {
	void Menu::IMenuItem::Create(App::StringID string_id, int index) {
		m_string_id = string_id;
		m_index = index;
	}

	Menu::IMenuItem::IMenuItem():
		m_string_id{ App::SIDInvalid },
		m_index{ 0 }
	{}

	std::wstring Menu::IMenuItem::String() {
		return UTF8ToWString(Intl::GetString(m_string_id));
	}

	int Menu::IMenuItem::Index() {
		return m_index;
	}
}
