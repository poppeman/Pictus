#include "StdAfx.h"
#include "resize_position_method.h"

#pragma warning(disable: 4512)
#pragma warning(disable: 4503)

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace App {
	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<ResizePositionMethod>> ResizePositionMethodIdentifierBimap;
	const ResizePositionMethodIdentifierBimap cg_resizePositionMethodIdentifiers = boost::assign::list_of<ResizePositionMethodIdentifierBimap::relation>
		("PositionToScreen", ResizePositionMethod::PositionToScreen)
		("PositionToCurrent", ResizePositionMethod::PositionToCurrent)
		("PositionNothing", ResizePositionMethod::PositionNothing)
		;

	ResizePositionMethod IdentifierToResizePositionMethod(std::string identifier) {
		return cg_resizePositionMethodIdentifiers.left.find(identifier)->second;
	}

	std::string ResizePositionMethodToIdentifier(ResizePositionMethod method) {
		return cg_resizePositionMethodIdentifiers.right.find(method)->second;
	}
}
