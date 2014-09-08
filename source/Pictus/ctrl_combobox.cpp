#include "StdAfx.h"
#include "ctrl_combobox.h"

namespace Win {
	void ComboBox::AddItem(const int sid, DWORD val) {
		ComboEntry e = ComboEntry(sid, val);
		m_entries.push_back(e);
		AddToComboBox(e);
	}

	void ComboBox::AddItem(const std::wstring& str, DWORD val) {
		ComboEntry e = ComboEntry(str, val);
		m_entries.push_back(e);
		AddToComboBox(e);
	}

	void ComboBox::SetSelection(DWORD val) {
		// Look for the correct item
		DWORD items = static_cast<DWORD>(SendMessage(Handle(), CB_GETCOUNT, 0, 0));

		for (DWORD i = 0; i < items; i++) {
			DWORD curr = static_cast<DWORD>(SendMessage(Handle(), CB_GETITEMDATA, i, 0));
			if (curr == val) {
				SendMessage(Handle(), CB_SETCURSEL, i, 0);
				return;
			}
		}
	}

	DWORD ComboBox::GetSelection() {
		return static_cast<DWORD>(SendMessage(Handle(),CB_GETCURSEL, 0, 0));
	}

	DWORD ComboBox::GetSelectionData() {
		return static_cast<DWORD>(SendMessage(Handle(), CB_GETITEMDATA, GetSelection(), 0));
	}

	void ComboBox::Reset() {
		SendMessage(Handle(), CB_RESETCONTENT, 0, 0); 
	}

	void ComboBox::Rebuild() {
		DWORD sel = GetSelectionData();
		Reset();
		std::for_each(m_entries.begin(), m_entries.end(), bind(&ComboBox::AddToComboBox, this, _1));
		SetSelection(sel);
	}

	void ComboBox::AddToComboBox(const ComboEntry& entry) {
		DWORD index = static_cast<DWORD>(SendMessage(Handle(), CB_ADDSTRING, 0, (LPARAM)entry.GetWString().c_str()));
		SendMessage(Handle(), CB_SETITEMDATA, index, entry.GetValue());
	}

	ComboBox::ComboBox(int id, HWND hwnd) :Control(id, hwnd) {
		m_lang = Intl::OnLanguageChanged.connect([&]() { Rebuild(); });
	}
	std::wstring ComboBox::ComboEntry::GetWString() const {
		if(SId == -1)
			return Contents;

		return Intl::GetWString(SId);
	}

	DWORD ComboBox::ComboEntry::GetValue() const {
		return Value;
	}

	ComboBox::ComboEntry::ComboEntry(int id, DWORD val)
		:SId(id),
		Value(val)
	{}

	ComboBox::ComboEntry::ComboEntry(const std::wstring& str, DWORD val):SId(-1), Contents(str), Value(val) {

	}

}
