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

		struct ComboEntry {
			std::wstring GetWString() const;
			DWORD GetValue() const;

			ComboEntry(int id, DWORD val);
			ComboEntry(const std::wstring& str, DWORD val);

		private:
			// Only one of these are used.
			int SId;
			std::wstring Contents;

			DWORD Value;
		};

		void AddToComboBox(const ComboEntry& entry);

		typedef std::vector<ComboEntry> ComboEntryVector;
		ComboEntryVector m_entries;
	};
}

#endif
