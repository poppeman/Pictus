#include "ctrl_listview.h"
#include <CommCtrl.h>
#include <Uxtheme.h>

namespace Win {
	void ListView::ClearColumns() {
		for (const auto& i : m_cols) {
			ListView_DeleteColumn(Handle(), i.first);
		}
		m_cols.clear();
	}

	void ListView::ClearItems() {
		ListView_DeleteAllItems(Handle());
	}

	bool ListView::AddColumn(const std::wstring& name, int width, int index) {
		if (AddEmptyColumn(width, index)) {
			m_cols[index].ExplicitTitle = name;
			Rebuild();

			return true;
		}
		return false;
	}

	bool ListView::AddColumn(App::StringID id, int width, int index) {
		if (AddEmptyColumn(width, index)) {
			m_cols[index].StringIDTitle = id;
			Rebuild();
			return true;
		}
		return false;
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
		m_lang = Intl::OnLanguageChanged.connect([&]() { Rebuild(); });
	}

	void ListView::Rebuild() {
		for (auto col : m_cols) {
			LVCOLUMN lvc;
			ZeroMemory(&lvc, sizeof(lvc));
			lvc.mask = LVCF_TEXT;

			if (col.second.StringIDTitle != App::SIDInvalid) {
				lvc.pszText = const_cast<wchar_t*>(Intl::GetWString(col.second.StringIDTitle));
			}
			else {
				lvc.pszText = const_cast<wchar_t*>(col.second.ExplicitTitle.c_str());
			}

			ListView_SetColumn(Handle(), col.first, &lvc);
		}
	}

	bool ListView::AddEmptyColumn(int width, int index) {
		LVCOLUMN lvc;
		ZeroMemory(&lvc, sizeof(lvc));

		// We must set a string, even if empty. Otherwise it will not be possible to change the title of
		// the column later on. Uncertain if it's a bug or a feature.
		lvc.mask = LVCF_WIDTH | LVCF_TEXT;
		lvc.pszText = L"";
		lvc.cx = width;
		auto ret = ListView_InsertColumn(Handle(), index, &lvc);
		if (ret != -1) {
			m_cols[index] = { L"", App::SIDInvalid };
			return true;
		}
		return false;
	}
}
