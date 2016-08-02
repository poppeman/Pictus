#ifndef PICTUS_RESIZE_POSITION_METHOD_H
#define PICTUS_RESIZE_POSITION_METHOD_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
#endif
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

	std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const App::ResizePositionMethod& fmt);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
