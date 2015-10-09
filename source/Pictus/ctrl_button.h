#ifndef CTRL_BUTTON_H
#define CTRL_BUTTON_H

#include "control.h"
#pragma warning(push)
#pragma warning(disable: 4512)
#include <boost/signals2.hpp>

namespace Win {
	class Button:public Control {
	public:
		boost::signals2::signal<void()> OnClick;

		Button(const Button&) = delete;
		Button& operator=(const Button&) = delete;

	private:
		Button() = delete;
		Button(int id, HWND hwnd);
		friend class BaseWindow;
	};
}

#pragma warning(pop)

#endif
