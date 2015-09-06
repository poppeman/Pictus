#ifndef CTRL_BUTTON_H
#define CTRL_BUTTON_H

#include "control.h"

namespace Win {
	class Button:public Control {
	public:
		boost::signals2::signal<void()> OnClick;

	private:
		Button() = delete;
		Button(int id, HWND hwnd);
		friend class BaseWindow;
	};
}

#endif
