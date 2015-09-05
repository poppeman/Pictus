#include "StdAfx.h"
#include "zoom_mode.h"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace App {
	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<ZoomMode>> ZoomModeIdentifierBimap;
	const ZoomModeIdentifierBimap cg_zoomModeIdentifiers = boost::assign::list_of<ZoomModeIdentifierBimap::relation>
		("ZoomFree", App::ZoomFree)
		("ZoomFitImage", App::ZoomFitImage)
		("ZoomFullSize", App::ZoomFullSize)
		;

	App::ZoomMode IdentifierToZoomMode(std::string identifier) {
		return cg_zoomModeIdentifiers.left.find(identifier)->second;
	}

	std::string ZoomModeToIdentifier(ZoomMode mode) {
		return cg_zoomModeIdentifiers.right.find(mode)->second;
	}
}
