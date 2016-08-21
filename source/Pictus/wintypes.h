#ifndef PICTUS_WINTYPES_H
#define PICTUS_WINTYPES_H

#include "app_types.h"
#include <wx/event.h>

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
		MouseEvent(wxMouseEvent& evt);
	};

	class KeyEvent {
	public:
		//WPARAM Key;
		int Key;
		bool AltPressed;
		bool CtrlPressed;
		bool ShiftPressed;

		bool operator<(const KeyEvent &rhs) const;

		KeyEvent(int key, bool isAltPressed, bool isCtrlPressed, bool isShiftPressed);
		KeyEvent(wxKeyEvent& evt);
	};

	std::string LongPath(const std::string &path);
	Geom::RectInt wxToRect(const wxRect &rect);
//	RECT RectToWx(const Geom::RectInt &rect);
	wxPoint PointToWx(const Geom::PointInt &point);
	Geom::PointInt wxToPoint(wxPoint pt);
	::Geom::SizeInt wxToSize(::wxSize sz);

	wxString GetStringWx(int id);
}

#endif
