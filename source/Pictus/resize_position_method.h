#ifndef PICTUS_RESIZE_POSITION_METHOD_H
#define PICTUS_RESIZE_POSITION_METHOD_H

#pragma warning(push)
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

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

#pragma warning(pop)

#endif
