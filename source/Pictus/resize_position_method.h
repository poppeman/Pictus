#ifndef PICTUS_RESIZE_POSITION_METHOD_H
#define PICTUS_RESIZE_POSITION_METHOD_H

namespace App {
	enum class ResizePositionMethod {
		PositionToScreen,
		PositionToCurrent,
		PositionNothing
	};

	ResizePositionMethod IdentifierToResizePositionMethod(std::string identifier);
	std::string ResizePositionMethodToIdentifier(ResizePositionMethod method);
}

#endif
