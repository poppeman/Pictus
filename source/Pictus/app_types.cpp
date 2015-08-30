#include "StdAfx.h"
#include "app_types.h"
#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include "../deps/orz/intl.h"

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
		("RemoveFile", KeyAction::DeleteCurrentFile)
		("RecycleFile", KeyAction::RecycleCurrentFile)
		("RemoveImage", KeyAction::RemoveCurrentImage)
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

	const std::map<KeyAction, App::StringID> cg_actionSids = {
		{ App::KeyAction::CloseApplication, SIDActionNextImage },
		{ App::KeyAction::NextImage, SIDActionNextImage },
		{ App::KeyAction::NextSkipImage, SIDActionNextSkipImage },
		{ App::KeyAction::PreviousImage, SIDActionPrevImage },
		{ App::KeyAction::PreviousSkipImage, SIDActionPrevSkipImage },
		{ App::KeyAction::RandomImage, SIDRandom },
		{ App::KeyAction::RenameFile, SIDRenameFilename },
		{ App::KeyAction::DeleteCurrentFile, SIDActionDeleteCurrentFile },
		{ App::KeyAction::RecycleCurrentFile, SIDActionRecycleCurrentFile },
		{ App::KeyAction::RemoveCurrentImage, SIDActionRemoveCurrentImage },
		{ App::KeyAction::OpenSettings, SIDSettings },
		{ App::KeyAction::PanUp, SIDActionPanUp },
		{ App::KeyAction::PanDown, SIDActionPanDown },
		{ App::KeyAction::PanLeft, SIDActionPanLeft },
		{ App::KeyAction::PanRight, SIDActionPanRight },
		{ App::KeyAction::ToggleFullscreen, SIDActionToggleFullScreen },
		{ App::KeyAction::FirstImage, SIDActionFirstImage },
		{ App::KeyAction::LastImage, SIDActionLastImage },
		{ App::KeyAction::ZoomIn, SIDZoomIn },
		{ App::KeyAction::ZoomOut, SIDZoomOut },
		{ App::KeyAction::ZoomDefault, SIDZoomDefault },
		{ App::KeyAction::ZoomFree, SIDZoomFree },
		{ App::KeyAction::ZoomFull, SIDZoomFullSize }
	};

	App::StringID KeyActionSid(KeyAction action) {
		return cg_actionSids.find(action)->second;
	}

	std::string KeyActionToIdentifier(KeyAction action) {
		auto it = cg_actionIdentifiers.right.find(action);
		return it->second;
	}

	KeyAction IdentifierToKeyAction(std::string identifier) {
		auto it = cg_actionIdentifiers.left.find(identifier);
		return it->second;
	}

	std::wstring GetKeyString(wchar_t key) {
		using namespace Intl;

		switch (key) {
		case VK_UP:
			return GetWString(SIDZoom);
		/*case VK_DOWN:
			return GetWString(SIDKeyDown);
		case VK_LEFT:
			return GetWString(SIDKeyLeft);
		case VK_RIGHT:
			return GetWString(SIDKeyRight);*/
		}

		std::wstring tmp;
		tmp += key;
		return tmp;
	}

}
