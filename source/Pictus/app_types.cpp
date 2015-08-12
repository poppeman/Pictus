#include "StdAfx.h"
#include "app_types.h"
#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace App {
	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<KeyAction>> ActionIdentifierBimap;
	const ActionIdentifierBimap cg_actionIdentifiers = boost::assign::list_of<ActionIdentifierBimap::relation>
		( "NextImage", App::KeyAction::NextImage )
		( "PrevImage", App::KeyAction::PreviousImage );

	App::StringID KeyActionSid(KeyAction action) {
		switch (action) {
		case App::KeyAction::NextImage:
			return SIDActionNextImage;;
		case App::KeyAction::PreviousImage:
			return SIDActionPrevImage;
		default:
			return SIDInvalid;
		}
	}

	std::string KeyActionToIdentifier(KeyAction action) {
		auto it = cg_actionIdentifiers.right.find(action);
		return it->second;
	}

	KeyAction IdentifierToKeyAction(std::string identifier) {
		auto it = cg_actionIdentifiers.left.find(identifier);
		return it->second;
	}
}
