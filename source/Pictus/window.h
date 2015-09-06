#ifndef WINDOW_H
#define WINDOW_H

#include "basewin.h"
#include "cursor.h"
#include "control.h"

namespace Win {
	class Window:public BaseWindow {
	public:
		boost::signals2::signal<void(int)> OnTaskbarButton;
		boost::signals2::signal<bool(Win::KeyEvent)> OnKeyDown;

		bool Create(BaseWindow* pParent);

		void Add(Control::Ptr control);

		void AlwaysOnTop(bool doAlwaysOnTop);
		void SetCursor(const Cursor& newCursor);

		void StartApplication();
		bool OnDropFiles(const StringVector& files);
		bool OnCreateTaskbar();

		bool OnWindowCreate();

		explicit Window();
		virtual ~Window();

	protected:
		friend class Dialog;

		bool ConstructWindow(const Geom::RectInt& position, DWORD exStyle, const wchar_t* className, const wchar_t* caption, DWORD styles);

	private:
		// Disable copy and assignment
		Window(const Window&);
		Window& operator=(const Window&);

		virtual bool PerformOnCreate()=0;

		virtual bool PerformOnWindowCreate();

		virtual bool PerformOnDropFiles(const StringVector&);
		virtual bool PerformOnCreateTaskbar();
		
	protected:
		LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		bool m_isAlwaysOnTop;

		typedef std::list<Control::Ptr> ControlList;
		ControlList m_controls;

		typedef std::set<HWND> DialogHandleMap;
		DialogHandleMap m_dialogsModeless;

		Cursor m_cursor;
		static UINT WM_TaskbarButtonCreated;
	};
}

#endif
