#include "StdAfx.h"
#include "ctrl_combobox.h"
#include "ctrl_combobox_entry.h"

namespace Win {
	void ComboBox::AddItem(const int sid, DWORD val) {
		Entry e = { sid, val };
		m_entries.push_back(e);
		AddToComboBox(e);
	}

	void ComboBox::AddItem(const std::wstring& str, DWORD val) {
		Entry e = { str, val };
		m_entries.push_back(e);
		AddToComboBox(e);
	}

	void ComboBox::SetSelection(DWORD val) {
		// Look for the correct item
		auto items = static_cast<DWORD>(SendMessage(Handle(), CB_GETCOUNT, 0, 0));

		for (DWORD i = 0; i < items; i++) {
			auto curr = static_cast<DWORD>(SendMessage(Handle(), CB_GETITEMDATA, i, 0));
			if (curr == val) {
				SendMessage(Handle(), CB_SETCURSEL, i, 0);
				// This SHOULDN'T be necessary, but it is and I can't be arsed to figure out why.
				// The combo box does not automatically get redrawn without this, even if the redraw flag is explicitly enabled
				// through WM_SETREDRAW.
				InvalidateRect(Handle(), nullptr, false);
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
		auto sel = GetSelectionData();
		Reset();

		for (auto e : m_entries) {
			AddToComboBox(e);
		}

		SetSelection(sel);
	}

	void ComboBox::AddToComboBox(const Entry& entry) {
		auto index = static_cast<DWORD>(SendMessage(Handle(), CB_ADDSTRING, 0, (LPARAM)entry.GetWString().c_str()));
		SendMessage(Handle(), CB_SETITEMDATA, index, entry.GetValue());
	}

	ComboBox::ComboBox(int id, HWND hwnd):
		Control{ id, hwnd }
	{
		m_lang = Intl::OnLanguageChanged.connect([&]() { Rebuild(); });
	}
}
