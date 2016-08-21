#include "wintypes.h"

#include <boost/scoped_array.hpp>
#include <orz/intl.h>

namespace Win {
	Geom::RectInt wxToRect(const wxRect& rect) {
		return Geom::RectInt(Geom::PointInt(rect.GetLeft(), rect.GetTop()), Geom::SizeInt(rect.GetRight()-rect.GetLeft(), rect.GetBottom()-rect.GetTop()));
	}

/*	RECT RectToRECT(const Geom::RectInt& rect) {
		RECT out_rect;
		out_rect.bottom	= rect.Bottom();
		out_rect.left	= rect.Left();
		out_rect.right	= rect.Right();
		out_rect.top	= rect.Top();
		return out_rect;
	}*/

	wxPoint PointToWx(const Geom::PointInt& point) {
		wxPoint out_point;
		out_point.x = point.X;
		out_point.y = point.Y;
		return out_point;
	}

	std::string LongPath(const std::string& path) {
		if (path.empty())
		{
			return path;
		}

		std::string trimmedPath;

		if ((path[0] == '\"') && ((path[path.length() - 1]) == '\"'))
		{
			trimmedPath = path.substr(1, path.length() - 2);
		}
		else
		{
			trimmedPath = path;
		}

#ifdef _WIN32
		// Convert the (potentially) short path to a long one.
		wchar_t single_char;
		uint32_t len			= GetLongPathName(UTF8ToWString(trimmedPath).c_str(), &single_char, 1);
		boost::scoped_array<wchar_t> converted(0);

		// Function APPARENTLY can't handle long pathnames (surrounded by quotes)
		// Here's a crazy idea. HOW ABOUT OUTPUTTING THE ORIGINAL PATH IF THE
		// INPUT STRING ACTUALLY IS A LONG PATH!? ALSO, CAPS-LOCK IS CRUISE CONTROL FOR COOL!

		// A few years later, I still do not regret the previous comment
		if (len != 0)
		{
			converted.reset(new wchar_t[len]);
			GetLongPathName(UTF8ToWString(trimmedPath).c_str(), converted.get(), len);
			return WStringToUTF8(converted.get());
		}
#else
		return trimmedPath;
#endif
	}

	::Geom::SizeInt wxToSize(::wxSize sz)
	{
		return {sz.x, sz.y};
	}

	Geom::PointInt wxToPoint(wxPoint pt)
	{
		return Geom::Point<int>(pt.x, pt.y);
	}

	wxString GetStringWx(int id)
	{
		return wxString::FromUTF8(Intl::GetString(id));
	}

	bool KeyEvent::operator<(const KeyEvent& rhs) const {
		if(!AltPressed && rhs.AltPressed) return true;
		if(AltPressed && !rhs.AltPressed) return false;
		if(!ShiftPressed && rhs.ShiftPressed) return true;
		if(ShiftPressed && !rhs.ShiftPressed) return false;
		if(!CtrlPressed && rhs.CtrlPressed) return true;
		if(CtrlPressed && !rhs.CtrlPressed) return false;
		return (Key < rhs.Key);
	}

	KeyEvent::KeyEvent(int key, bool isAltPressed, bool isCtrlPressed, bool isShiftPressed):
		Key(key),
		AltPressed(isAltPressed),
		CtrlPressed(isCtrlPressed),
		ShiftPressed(isShiftPressed)
	{
	}

	KeyEvent::KeyEvent(wxKeyEvent& evt):
		KeyEvent(evt.GetKeyCode(), evt.AltDown(), evt.CmdDown(), evt.ShiftDown())
	{
	}

	MouseEvent::MouseEvent() : Button(Left), Position(0, 0), WheelTicks(0)
	{

	}

	MouseEvent::MouseEvent(wxMouseEvent& evt):
		Button(MouseButton::NoButton),
		Position(evt.GetPosition().x, evt.GetPosition().y),
		WheelTicks(evt.GetWheelRotation())
	{
		if(evt.Button(wxMOUSE_BTN_LEFT))
		{
			Button = MouseButton::Left;
		}
		if(evt.Button(wxMOUSE_BTN_RIGHT))
		{
			Button = MouseButton::Right;
		}
		if(evt.Button(wxMOUSE_BTN_MIDDLE))
		{
			Button = MouseButton::Middle;
		}
		if(evt.Button(wxMOUSE_BTN_AUX1))
		{
			Button = MouseButton::Extra1;
		}
		if(evt.Button(wxMOUSE_BTN_AUX2))
		{
			Button = MouseButton::Extra2;
		}

	}

}
