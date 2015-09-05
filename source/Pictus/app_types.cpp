#include "StdAfx.h"
#include "app_types.h"
#include "../deps/orz/intl.h"
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
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
		{ App::KeyAction::CloseApplication, SIDActionClose },
		{ App::KeyAction::NextImage, SIDActionNextImage },
		{ App::KeyAction::NextSkipImage, SIDActionNextSkipImage },
		{ App::KeyAction::PreviousImage, SIDActionPreviousImage },
		{ App::KeyAction::PreviousSkipImage, SIDActionPreviousSkipImage },
		{ App::KeyAction::RandomImage, SIDActionRandomImage },
		{ App::KeyAction::RenameFile, SIDActionRenameFile },
		{ App::KeyAction::DeleteCurrentFile, SIDActionDeleteCurrentFile },
		{ App::KeyAction::RecycleCurrentFile, SIDActionRecycleCurrentFile },
		{ App::KeyAction::RemoveCurrentImage, SIDActionRemoveCurrentImage },
		{ App::KeyAction::OpenSettings, SIDSettings },
		{ App::KeyAction::PanUp, SIDActionPanUp },
		{ App::KeyAction::PanDown, SIDActionPanDown },
		{ App::KeyAction::PanLeft, SIDActionPanLeft },
		{ App::KeyAction::PanRight, SIDActionPanRight },
		{ App::KeyAction::ToggleFullscreen, SIDActionToggleFullscreen },
		{ App::KeyAction::FirstImage, SIDActionFirstImage },
		{ App::KeyAction::LastImage, SIDActionLastImage },
		{ App::KeyAction::ZoomIn, SIDActionZoomIn },
		{ App::KeyAction::ZoomOut, SIDActionZoomOut },
		{ App::KeyAction::ZoomDefault, SIDActionZoomDefault },
		{ App::KeyAction::ZoomFree, SIDActionZoomFree },
		{ App::KeyAction::ZoomFull, SIDActionZoomFullSize },
		{ App::KeyAction::CopyImage, SIDActionCopyToClipboard }
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
			return GetWString(SIDKeysUpArrow);
		case VK_DOWN:
			return GetWString(SIDKeysDownArrow);
		case VK_LEFT:
			return GetWString(SIDKeysLeftArrow);
		case VK_RIGHT:
			return GetWString(SIDKeysRightArrow);
		}

		std::wstring tmp;
		tmp += key;
		return tmp;
	}
}
