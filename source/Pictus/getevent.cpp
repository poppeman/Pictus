#include "StdAfx.h"
#include "getevent.h"
#include "registry.h"

namespace App {
	using namespace Reg::Keys;

	MouseAction MouseStandardEvent(const Win::MouseEvent& e) {
		switch(e.Button) {
		case Win::Left:
			return MouseAction(Reg::Key(DWOnMouseLeft));
		case Win::Right:
			return MouseAction(Reg::Key(DWOnMouseRight));
		case Win::Middle:
			return MouseAction(Reg::Key(DWOnMouseMiddle));
		case Win::Extra1:
			return MouseAction(Reg::Key(DWOnMouseWheelUp));
		case Win::Extra2:
			return MouseAction(Reg::Key(DWOnMouseWheelDown));
		default:
			// Might be a scrollwheel action
			if (e.WheelTicks>0)
				return MouseAction(Reg::Key(DWOnMouseWheelUp));
			else if (e.WheelTicks<0)
				return MouseAction(Reg::Key(DWOnMouseWheelDown));

			// Nope, undefined!
			return MouseUndefined;
		}
	}

	MouseAction MouseDblEvent(const Win::MouseEvent& e) {
		switch(e.Button) {
			case Win::Left:
				return MouseAction(Reg::Key(DWOnMouseLeftDbl));
			case Win::Right:
				return MouseAction(Reg::Key(DWOnMouseRightDbl));
			case Win::Middle:
				return MouseAction(Reg::Key(DWOnMouseMiddleDbl));
			default:
				return MouseUndefined;
		}
	}
}
