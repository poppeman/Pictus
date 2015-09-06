#ifndef DIALOG_H
#define DIALOG_H

#include "basewin.h"
#include "orz/intl.h"

namespace Win {
	class ListView;
	class EditBox;
	class ComboBox;

	class Dialog:public BaseWindow {
	public:
		bool DoModal(Win::BaseWindow* pParent);
		bool DoModeless(Win::BaseWindow* pParent);
		bool IsModelessVisible();

		bool OnInitDialog();

		explicit Dialog(int id);
		virtual ~Dialog()=0;

		bool GetCheckBox(DWORD id);
		void SetCheckBox(DWORD id, bool state);

		DWORD GetEditBoxNum(DWORD id);
		void SetEditBoxNum(DWORD id, DWORD val);

		void ControlText(DWORD id, const std::wstring& text);
		void ControlText(DWORD id, int sid);

		void ShowControl(DWORD id, bool doShow);

		LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		bool OnNotify(DWORD id, LPNMHDR pnmh);

	private:
		void UpdateControlStrings();

		virtual void RegisterWindowClasses();
		virtual void PerformOnCreate();
		virtual bool PerformOnNotify(DWORD id, LPNMHDR pnmh);
		virtual bool PerformOnInitDialog();

		typedef std::map<int, int> ControlTextSIdVector;
		ControlTextSIdVector m_controlSIds;

		boost::signals2::connection m_lang;

		unsigned int m_iTemplateID;
	};
}

#endif
