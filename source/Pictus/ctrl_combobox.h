#ifndef CTRL_COMBOBOX_H
#define CTRL_COMBOBOX_H

#include "control.h"
#include "orz/intl.h"

namespace Win {
	class ComboBox:public Control {
	public:
		boost::signals2::signal<void()> OnSelectionChanged;

		void AddItem(const int sid, DWORD val);
		void AddItem(const std::wstring& str, DWORD val);
		void SetSelection(DWORD val);
		DWORD GetSelectionData();

		void Reset();

	private:
		DWORD GetSelection();

		ComboBox() = delete;
		ComboBox(int id, HWND hwnd);
		friend class BaseWindow;

		boost::signals2::connection m_lang;
		void Rebuild();

		struct Entry;

		void AddToComboBox(const Entry& entry);

		typedef std::vector<Entry> ComboEntryVector;
		ComboEntryVector m_entries;
	};
}

#endif
