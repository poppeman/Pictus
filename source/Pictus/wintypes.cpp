#include "wintypes.h"

#include <boost/scoped_array.hpp>

namespace Win {
	Geom::RectInt RECTToRect(const RECT& rect) {
		return Geom::RectInt(Geom::PointInt(rect.left, rect.top), Geom::SizeInt(rect.right-rect.left, rect.bottom-rect.top));
	}

	RECT RectToRECT(const Geom::RectInt& rect) {
		RECT out_rect;
		out_rect.bottom	= rect.Bottom();
		out_rect.left	= rect.Left();
		out_rect.right	= rect.Right();
		out_rect.top	= rect.Top();
		return out_rect;
	}

	POINT PointToPOINT(const Geom::PointInt& point) {
		POINT out_point;
		out_point.x = point.X;
		out_point.y = point.Y;
		return out_point;
	}

	std::wstring LongPath(const std::wstring& path) {
		if (path.empty()) return path;

		std::wstring trimmedPath;

		if ((path[0] == L'\"') && ((path[path.length() - 1]) == L'\"'))
			trimmedPath = path.substr(1, path.length() - 2);
		else
			trimmedPath = path;

		// Convert the (potentially) short path to a long one.
		wchar_t single_char;
		uint32_t len			= GetLongPathName(trimmedPath.c_str(), &single_char, 1);
		boost::scoped_array<wchar_t> converted(0);

		// Function APPARENTLY can't handle long pathnames (surrounded by quotes)
		// Here's a crazy idea. HOW ABOUT OUTPUTTING THE ORIGINAL PATH IF THE
		// INPUT STRING ACTUALLY IS A LONG PATH!? ALSO, CAPS-LOCK IS CRUISE CONTROL FOR COOL!
		if (len != 0) {
			converted.reset(new wchar_t[len]);
			GetLongPathName(trimmedPath.c_str(), converted.get(), len);
			return converted.get();
		}
		return trimmedPath;
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

	KeyEvent::KeyEvent(WPARAM key, bool isAltPressed, bool isCtrlPressed, bool isShiftPressed)
		:Key(key),
		AltPressed(isAltPressed),
		CtrlPressed(isCtrlPressed),
		ShiftPressed(isShiftPressed)
	{
	}
}
