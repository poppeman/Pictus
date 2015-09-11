#include "StdAfx.h"
#include "ctrl_listview.h"

namespace Win {
	void ListView::Clear() {
		ListView_DeleteAllItems(Handle());

		for (const auto& i : m_cols) {
			ListView_DeleteColumn(Handle(), i);
		}

		m_cols.clear();
	}

	int ListView::AddColumn(const std::wstring& name, const int width, int index) {
		LVCOLUMN lvc;
		ZeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT | LVCF_WIDTH;
		lvc.pszText = (wchar_t*)name.c_str(); // Doesn't write, this cast _shouldn't_ be dangerous
		lvc.cx = width;
		if (index!=0) m_cols.insert(index);
		return ListView_InsertColumn(Handle(), index, &lvc);
	}

	void ListView::ColumnName(const int index, const std::wstring& name) {
		LVCOLUMN lvc;
		ZeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT;
		lvc.pszText = (wchar_t*)name.c_str(); // Doesn't write, this cast _shouldn't_ be dangerous
		SendMessage(Handle(), LVM_SETCOLUMN, index, (LPARAM)&lvc);
	}

	int ListView::AddItem(const std::wstring& caption, LPARAM data) {
		auto index = Size();

		LVITEM lvi;
		ZeroMemory(&lvi, sizeof(lvi));
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.pszText = (wchar_t*)caption.c_str();
		lvi.iItem = index;
		lvi.lParam = data;
		ListView_InsertItem(Handle(), &lvi);

		return index;
	}

	bool ListView::RemoveItem(int row) {
		return ListView_DeleteItem(Handle(), row) == TRUE;
	}

	void ListView::ItemColumn(const int row, const int column, const std::wstring& caption) {
		LVITEM lvi;
		ZeroMemory(&lvi, sizeof(lvi));
		lvi.mask = LVIF_TEXT;
		lvi.pszText = (wchar_t*)caption.c_str();
		lvi.iItem = row;
		lvi.iSubItem = column;
		ListView_SetItem(Handle(), &lvi);
	}

	int ListView::GetSelectedRow() const {
		return ListView_GetNextItem(Handle(), -1, LVNI_SELECTED);
	}

	void ListView::SetSelectedRow(int index) {
		ListView_SetItemState(Handle(), index, LVIS_FOCUSED | LVIS_SELECTED, 0x000f);
		ListView_EnsureVisible(Handle(), index, false);
	}

	LPARAM ListView::GetItemParam(int row) const {
		LVITEM lvi;
		ZeroMemory(&lvi, sizeof(lvi));
		lvi.mask = LVIF_PARAM;
		lvi.iItem = row;
		ListView_GetItem(Handle(), &lvi);
		return lvi.lParam;
	}

	int ListView::Size() const {
		return ListView_GetItemCount(Handle());
	}

	void ListView::Check(size_t index, bool doCheck) {
		ListView_SetCheckState(Handle(), index, doCheck);
	}

	bool ListView::IsChecked(size_t index) {
		return ListView_GetCheckState(Handle(), index)!=0;
	}

	void ListView::Style(DWORD flags) {
		ListView_SetExtendedListViewStyle(Handle(), flags);
	}

	boost::optional<LRESULT> ListView::PerformOnNotify(LPNMHDR lParam) {
		if (lParam->code == LVN_ITEMCHANGED) {
			auto nmlv = reinterpret_cast<LPNMLISTVIEW>(lParam);
			if (nmlv->uNewState & LVIS_SELECTED) {
				if (OnSelectionChanged != nullptr) {
					OnSelectionChanged(nmlv->iItem);
					return (LRESULT)1;
				}
			}
		}
		return (LRESULT)0;
	}

	ListView::ListView(int id, HWND hwnd):
		Control{ id, hwnd }
	{
		SetWindowTheme(Handle(), L"Explorer", NULL);
	}

}
