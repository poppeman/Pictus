#ifndef RENAME_H
#define RENAME_H

#include "dialog.h"
#include "ctrl_editbox.h"

namespace App {
	class Rename:public Win::Dialog {
	public:
		const std::wstring& Name() { return m_name; }

		Rename(const std::wstring& currname);

	private:
		Win::EditBox* m_filename;

	private:
		bool PerformOnInitDialog();
		void OnOk();
		void OnCancel();

		std::wstring m_name;

		static WNDPROC m_prevEditWndProc;
		static LRESULT CALLBACK FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif
