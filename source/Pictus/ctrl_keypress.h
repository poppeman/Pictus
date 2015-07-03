#ifndef PICTUS_APP_CTRL_KEYPRESS_H
#define PICTUS_APP_CTRL_KEYPRESS_H

#include "control.h"

namespace App {
	class Keypress :public Win::Control {
	public:
		static Keypress* CreateKeypress(int id, HWND parent);

	protected:
		// Block invalid construction
		Keypress(int id, HWND hwnd);

		friend class BaseWindow;

	private:
		// Sub classing
		WNDPROC m_prevEditWndProc;

		static LRESULT CALLBACK	FilterEditWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};
}

#endif
