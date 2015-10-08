#include "getevent.h"
#include "registry.h"

namespace App {
	MouseAction MouseStandardEvent(const Win::MouseEvent& e, Reg::MouseSettings cfg) {
		switch(e.Button) {
		case Win::Left:
			return MouseAction(cfg.OnMouseLeft);
		case Win::Right:
			return MouseAction(cfg.OnMouseRight);
		case Win::Middle:
			return MouseAction(cfg.OnMouseMiddle);
		case Win::Extra1:
			return MouseAction(cfg.OnMouseWheelUp);
		case Win::Extra2:
			return MouseAction(cfg.OnMouseWheelDown);
		default:
			// Might be a scrollwheel action
			if (e.WheelTicks > 0) {
				return MouseAction(cfg.OnMouseWheelUp);
			}
			else if (e.WheelTicks < 0) {
				return MouseAction(cfg.OnMouseWheelDown);
			}

			// Nope, undefined!
			return MouseUndefined;
		}
	}

	MouseAction MouseDblEvent(const Win::MouseEvent& e, Reg::MouseSettings cfg) {
		switch(e.Button) {
			case Win::Left:
				return MouseAction(cfg.OnMouseLeftDbl);
			case Win::Right:
				return MouseAction(cfg.OnMouseRightDbl);
			case Win::Middle:
				return MouseAction(cfg.OnMouseMiddleDbl);
			default:
				return MouseUndefined;
		}
	}
}
