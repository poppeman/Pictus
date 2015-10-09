#include "appreg.h"
#include <ostream>

std::basic_ostream<char> &operator<<(std::basic_ostream<char>& in, const App::ResizePositionMethod& fmt) {
	switch (fmt) {
	case App::ResizePositionMethod::PositionToScreen:
		in << L"PositionToScreen";
		break;
	case App::ResizePositionMethod::PositionToCurrent:
		in << L"PositionToCurrent";
		break;
	case App::ResizePositionMethod::PositionNothing:
		in << L"PositionNothing";
		break;
	default:
		in << L"Unknown(" << static_cast<int>(fmt) << L")";
		break;
	}
	return in;
}
