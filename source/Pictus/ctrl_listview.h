#ifndef CTRL_LISTVIEW_H
#define CTRL_LISTVIEW_H

#include "control.h"

namespace Win {
	/**
	* Simple wrapper class for WinAPI listviews. Does not support multi-select listviews.
	*/
	class ListView:public Control {
	public:
		void Clear();

		int AddColumn(const std::wstring& name, const int width, const int index);
		void ColumnName(const int index, const std::wstring& name);

		int AddItem(const std::wstring& caption, LPARAM data=0);
		void ItemColumn(const int row, const int column, const std::wstring& caption);

		int GetSelectedRow() const;
		LPARAM GetItemParam(int row) const;
		int Size() const;

		void Check(size_t index, bool doCheck);
		bool IsChecked(size_t index);
		void Style(DWORD flags);

		std::function<void(int)> OnSelectionChanged;

	protected:
		ListView(int id, HWND hwnd);

		friend class BaseWindow;

	private:
		boost::optional<LRESULT> PerformOnNotify(LPNMHDR lParam) override;

		// Avoid unintentional creation
		ListView()=delete;

		int m_index;

		typedef std::set<int>	Columns;
		Columns m_cols;
	};
}

#endif
