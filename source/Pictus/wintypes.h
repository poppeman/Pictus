#ifndef WINTYPES_H
#define WINTYPES_H

#include "app_types.h"

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

		MouseEvent():Button(Left), Position(0, 0), WheelTicks(0) {}
		MouseEvent(_In_ LPARAM lParam):Position(static_cast<SHORT>(LOWORD(lParam)), static_cast<SHORT>(HIWORD(lParam))), WheelTicks(0) { }
	};

	class KeyEvent {
	public:
		WPARAM Key;
		bool AltPressed;
		bool CtrlPressed;
		bool ShiftPressed;

		bool operator <(const _In_ KeyEvent& rhs) const;

		KeyEvent(_In_ WPARAM key, _In_ bool isAltPressed, _In_ bool isCtrlPressed, _In_ bool isShiftPressed);
	};

	std::wstring LongPath(_In_ const std::wstring& path);

	Geom::RectInt RECTToRect(_In_ const RECT& rect);
	RECT RectToRECT(_In_ const Geom::RectInt& rect);

	POINT PointToPOINT(_In_ const Geom::PointInt& point);
};

#endif
