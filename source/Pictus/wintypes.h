#ifndef PICTUS_WINTYPES_H
#define PICTUS_WINTYPES_H

#include "app_types.h"
#include <wx/event.h>
#include <orz/intl_language.h>
#include <wx/colour.h>

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
		wchar_t Key;
		bool AltPressed;
		bool CtrlPressed;
		bool ShiftPressed;

		bool operator<(const KeyEvent &rhs) const;

		KeyEvent(wchar_t key, bool isAltPressed, bool isCtrlPressed, bool isShiftPressed);
		KeyEvent(wxKeyEvent& evt);
	};

	Geom::RectInt wxToRect(const wxRect &rect);
//	RECT RectToWx(const Geom::RectInt &rect);
	wxPoint PointToWx(const Geom::PointInt &point);
	Geom::PointInt wxToPoint(wxPoint pt);
	::Geom::SizeInt wxToSize(::wxSize sz);
	wxColor ColorToWx(Img::Color c);

	wxString GetStringWx(int id);
	wxString GetStringWx(int id, Intl::Language lang);
}

#endif
