#ifndef PICTUS_RESIZE_POSITION_METHOD_H
#define PICTUS_RESIZE_POSITION_METHOD_H

namespace App {
	enum class ResizePositionMethod {
		PositionToScreen,
		PositionToCurrent,
		PositionNothing
	};

	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<ResizePositionMethod>> ResizePositionMethodIdentifierBimap;
	const ResizePositionMethodIdentifierBimap ResizePositionMethodIdentifier = boost::assign::list_of<ResizePositionMethodIdentifierBimap::relation>
		("PositionToScreen", ResizePositionMethod::PositionToScreen)
		("PositionToCurrent", ResizePositionMethod::PositionToCurrent)
		("PositionNothing", ResizePositionMethod::PositionNothing)
		;
}

#endif
