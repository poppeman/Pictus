#ifndef PICTUS_VIEWER_KEYBOARD_H
#define PICTUS_VIEWER_KEYBOARD_H

#include "actionmap.h"
#include "wintypes.h"
#include "appreg.h"

namespace App {
	class Viewer;

	class ViewerKeyboard {
	public:
		void Construct(Viewer* owner);
		void SetBindings(Reg::KeyboardSettings cfg);

	private:
		Reg::KeyboardSettings m_cfg;
		Viewer* m_owner = nullptr;
	};
}

#endif
