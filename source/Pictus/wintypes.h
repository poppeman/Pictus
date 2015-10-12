#ifndef PICTUS_WINTYPES_H
#define PICTUS_WINTYPES_H

#include "app_types.h"
#include <windows.h>

namespace Win {
	enum MouseButton {
		NoButton,
		Left,
		Right,
		Middle,
		Extra1,
		Extra2
	};

	class MouseEvent {
	public:
		MouseButton Button;
		Geom::PointInt Position;
		int WheelTicks;

		MouseEvent();
		MouseEvent(LPARAM lParam);
	};

	class KeyEvent {
	public:
		WPARAM Key;
		bool AltPressed;
		bool CtrlPressed;
		bool ShiftPressed;

		bool operator<(const KeyEvent &rhs) const;

		KeyEvent(WPARAM key, bool isAltPressed, bool isCtrlPressed, bool isShiftPressed);
	};

	std::string LongPath(const std::string &path);
	Geom::RectInt RECTToRect(const RECT &rect);
	RECT RectToRECT(const Geom::RectInt &rect);
	POINT PointToPOINT(const Geom::PointInt &point);
}

#endif
