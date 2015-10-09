#include "window.h"
#include "ctrl_statusbar.h"

#include <shellapi.h>
#include <ShlObj.h>

namespace Win {
	using namespace Geom;

	UINT Window::WM_TaskbarButtonCreated = WM_NULL;

	bool Window::Create(BaseWindow* pParent) {
		Parent(pParent); return PerformOnCreate();
	}

	void Window::Add(Control::Ptr control) {
		COND_STRICT(control, Err::InvalidParam, "control was Null.");
		m_controls.push_back(control);
		control->AttachTo(this);
	}

	void Window::AlwaysOnTop( bool doAlwaysOnTop ) {
		if (m_isAlwaysOnTop == doAlwaysOnTop) return;

		HWND newPosition = (doAlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST);
		SetWindowPos(Handle(), newPosition, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		m_isAlwaysOnTop = doAlwaysOnTop;
	}

	void Window::SetCursor(const Cursor& newCursor) {
		m_cursor = newCursor;
		m_cursor.SetCursor();
	}

	bool Window::OnDropFiles(const StringVector& files) { return PerformOnDropFiles(files); }

	void Window::StartApplication() {
		MSG msg;
		BOOL bRet;
		while((bRet = GetMessage(&msg, 0, 0, 0)) != 0) {
			if (bRet == -1)
				break;

			bool wereDialogMessage = false;
			for(const auto& dialog : m_dialogsModeless) {
				if (IsDialogMessage(dialog, &msg)) {
					wereDialogMessage = true;
					break;
				}
			}

			if(wereDialogMessage == false) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	bool Window::OnWindowCreate() { return PerformOnWindowCreate(); }

	bool Window::ConstructWindow(const Geom::RectInt& position, DWORD exStyle, const wchar_t* className, const wchar_t* caption, DWORD styles) {
		HWND hParent=(Parent()?Parent()->Handle():0);
		if (exStyle & WS_EX_TOPMOST) m_isAlwaysOnTop = true;
		HWND h=CreateWindowEx(
			exStyle,
			className,
			caption,
			styles,
			position.Left(),
			position.Top(),
			position.Width(),
			position.Height(),
			hParent,
			0,
			GetHINSTANCE(),
			this);
		if(h == 0) return false;
		Handle(h);
		return true;
	}

	Window::Window():
		m_isAlwaysOnTop{ false }
	{}

	bool Window::PerformOnWindowCreate() {
		return true;
	}

	bool Window::PerformOnDropFiles(const StringVector&) {
		return false;
	}

	bool Window::PerformOnCreateTaskbar() {
		return false;
	}

	Window::~Window() {}

	LRESULT Window::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if(WM_TaskbarButtonCreated == WM_NULL)
			WM_TaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");

		if(msg == WM_TaskbarButtonCreated)
			OnCreateTaskbar();

		switch (msg) {
			case WM_COMMAND:
				if(HIWORD(wParam) == THBN_CLICKED) {
					OnTaskbarButton(LOWORD(wParam));
					return 0;
				}
				break;
			case WWM_REGISTERMODELESS:
				m_dialogsModeless.insert(reinterpret_cast<HWND>(wParam));
				break;
			case WWM_UNREGISTERMODELESS:
				m_dialogsModeless.erase(reinterpret_cast<HWND>(wParam));
				break;
			case WM_CREATE:    // Window creation message
				if(OnWindowCreate()) return 0;
				return -1;

			case WM_PAINT:     // Paint message
				if (OnPaint()) return 0;
				break;

			case WM_ERASEBKGND: // Erase background
				if (OnEraseBackground((HDC)wParam)) return 1; // Non-zero = erased
				break;

			case WM_CONTEXTMENU:  // Context menu should fire
				{
					if (OnContextMenu(PointInt(LOWORD(lParam), HIWORD(lParam)))) return 0;
					break;
				}
			case WM_SIZE:
				{
					for (auto p: m_controls) {
						if(dynamic_cast<StatusBar*>(p.get()))
							SendMessage(p->Handle(), WM_SIZE, 0, 0);
					}

					if (OnSize(SizeInt(LOWORD(lParam), HIWORD(lParam)))) return 0;
					break;
				}
			case WM_MOVE:
				{
					if (OnMove(PointInt(LOWORD(lParam), HIWORD(lParam)))) return 0;
					break;
				}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				{
					boost::optional<bool> rcv;
					bool isCtrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
					bool isShiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

					if((rcv = OnKeyDown(KeyEvent(wParam, msg == WM_SYSKEYDOWN, isCtrlPressed, isShiftPressed))) && *rcv) return 0;
					break;
				}
			case WM_SETCURSOR:
				{
					if (m_cursor.SetCursor()) return 0;
					break;
				}
			case WM_DROPFILES:
				{
					HDROP hDrop = reinterpret_cast<HDROP>(wParam);

					DWORD numStrings = DragQueryFile(hDrop, 0xFFFFFFFF, 0, 0);	// Get number of files dropped
					// Store filenames in a vector
					StringVector v(numStrings);
					for (DWORD i = 0; i < numStrings; ++i) {
						TCHAR tmpstr[1024];
						DragQueryFile(hDrop, i, tmpstr, 1024);
						v.at(i) = tmpstr;
					}
					DragFinish(hDrop);
					// Handle message
					if (OnDropFiles(v)) return 0;
					break;
				}

			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			case WM_NOTIFYFORMAT:
				return NFR_UNICODE;
		}

		return baseWndProc(hwnd, msg, wParam, lParam, true);
	}

	bool Window::OnCreateTaskbar() {
		return PerformOnCreateTaskbar();
	}

}
