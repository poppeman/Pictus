#include "resize_position_method.h"
#include <ostream>

namespace App {
	std::basic_ostream<char> &operator<<(std::basic_ostream<char>& in, const App::ResizePositionMethod& fmt) {
		switch (fmt) {
		case App::ResizePositionMethod::PositionToScreen:
			in << "PositionToScreen";
			break;
		case App::ResizePositionMethod::PositionToCurrent:
			in << "PositionToCurrent";
			break;
		case App::ResizePositionMethod::PositionNothing:
			in << "PositionNothing";
			break;
		default:
			in << "Unknown(" << static_cast<int>(fmt) << ")";
			break;
		}
		return in;
	}
}
