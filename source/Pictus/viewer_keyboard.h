#ifndef VIEWER_KEYBOARD_H
#define VIEWER_KEYBOARD_H

#include "actionmap.h"
#include "wintypes.h"
#include "appreg.h"

namespace App {
	class Viewer;

	class ViewerKeyboard {
	public:
		void Construct(Viewer* owner, Reg::KeyboardSettings cfg);

	private:
		Reg::KeyboardSettings m_cfg;
	};
}

#endif
