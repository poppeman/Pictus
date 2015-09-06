#ifndef PICTUS_CTRL_COMBOBOX_ENTRY_H
#define PICTUS_CTRL_COMBOBOX_ENTRY_H

#include "ctrl_combobox.h"

namespace Win {
	struct ComboBox::Entry {
		std::wstring GetWString() const;
		DWORD GetValue() const;

		Entry(int id, DWORD val);
		Entry(std::wstring str, DWORD val);

	private:
		// Only one of these are used.
		int SId;
		std::wstring Contents;

		DWORD Value;
	};
}

#endif
