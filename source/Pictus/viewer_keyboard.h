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

		//enum Flags {
		//	Shift = 0x1,
		//	Alt = 0x2,
		//	Ctrl = 0x4
		//};

		//void AddMapping(WPARAM key, KeyActionMap::Function_Type f, bool alt = false, bool shift = false, bool ctrl = false);
		//void AddMapping(WPARAM key, KeyActionMap::Function_Type f, int flags = 0);
	};
}

#endif
