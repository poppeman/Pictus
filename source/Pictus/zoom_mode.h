#ifndef PICTUS_ZOOM_MODE_H
#define PICTUS_ZOOM_MODE_H

#pragma warning(push)
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace App {
	enum ZoomMode {
		ZoomFree = 0,
		ZoomFitImage,
		ZoomFullSize,
	};

	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<ZoomMode>> ZoomModeIdentifierBimap;
	const ZoomModeIdentifierBimap ZoomModeIdentifier = boost::assign::list_of<ZoomModeIdentifierBimap::relation>
		("ZoomFree", App::ZoomFree)
		("ZoomFitImage", App::ZoomFitImage)
		("ZoomFullSize", App::ZoomFullSize)
		;
}
#pragma warning(pop)

#endif
