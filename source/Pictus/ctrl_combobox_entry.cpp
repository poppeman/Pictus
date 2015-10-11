#include "ctrl_combobox_entry.h"

namespace Win {
	std::wstring ComboBox::Entry::GetWString() const {
		if (SId == -1) {
			return Contents;
		}

		return UTF8ToWString(Intl::GetString(SId));
	}

	DWORD ComboBox::Entry::GetValue() const {
		return Value;
	}

	ComboBox::Entry::Entry(int id, DWORD val):
		SId{ id },
		Value{ val }
	{}

	ComboBox::Entry::Entry(std::wstring str, DWORD val):
		SId{ -1 },
		Contents( str ),
		Value{ val }
	{}
}
