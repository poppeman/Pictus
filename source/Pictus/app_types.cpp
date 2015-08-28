#include "StdAfx.h"
#include "app_types.h"
#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace App {
	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<KeyAction>> ActionIdentifierBimap;
	const ActionIdentifierBimap cg_actionIdentifiers = boost::assign::list_of<ActionIdentifierBimap::relation>
		("CloseApplication", KeyAction::CloseApplication)
		("NextImage", KeyAction::NextImage)
		("NextSkipImage", KeyAction::NextSkipImage)
		("PrevImage", KeyAction::PreviousImage)
		("PrevSkipImage", KeyAction::PreviousSkipImage)
		("RandomImage", KeyAction::RandomImage)
		("RenameFile", KeyAction::RenameFile)
		("RemoveFile", KeyAction::DeleteFile)
		("RecycleFile", KeyAction::RecycleFile)
		("RemoveImage", KeyAction::RemoveImage)
		("OpenSettings", KeyAction::OpenSettings)
		("PanUp", KeyAction::PanUp)
		("PanDown", KeyAction::PanDown)
		("PanLeft", KeyAction::PanLeft)
		("PanRight", KeyAction::PanRight)
		("ToggleFullscreen", KeyAction::ToggleFullscreen)
		("FirstImage", KeyAction::FirstImage)
		("LastImage", KeyAction::LastImage)
		("ZoomIn", KeyAction::ZoomIn)
		("ZoomOut", KeyAction::ZoomOut)
		("ZoomDefault", KeyAction::ZoomDefault)
		("ZoomFree", KeyAction::ZoomFree)
		("ZoomFull", KeyAction::ZoomFull)
		;

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
