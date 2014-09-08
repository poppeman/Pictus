#ifndef CTRL_LISTVIEW_H
#define CTRL_LISTVIEW_H

#include "control.h"

namespace Win {
	class ListView:public Control {
	public:
		void Clear();

		int AddColumn(const std::wstring& name, const int width, const int index);
		void ColumnName(const int index, const std::wstring& name);

		int AddItem(const std::wstring& caption);
		void ItemColumn(const int row, const int column, const std::wstring& caption);

		void Check(size_t index, bool doCheck);
		bool IsChecked(size_t index);
		void Style(DWORD flags);

	protected:
		ListView(int id, HWND hwnd):Control(id, hwnd), m_index(0) {}

		friend class BaseWindow;

	private:
		// Avoid unintentional creation
		ListView();

		int m_index;

		typedef std::set<int>	Columns;
		Columns m_cols;
	};
}

#endif
