#ifndef PICTUS_APP_CTRL_KEYPRESS_H
#define PICTUS_APP_CTRL_KEYPRESS_H

#include "control.h"
#include "app_types.h"

namespace App {
	class Keypress :public Win::Control {
	public:
		std::function<void(App::KeyboardPress)> OnNewCombo;
		void SetCombo(App::KeyboardPress kp);

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
