#include "app_types.h"
#include "../deps/orz/intl.h"
#ifdef _MSC_VER
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
#endif
#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <wx/event.h>

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
		("CopyImage", KeyAction::CopyImage)
		("ToggleFullSizeDefaultZoom", KeyAction::ToggleFullSizeDefaultZoom)
		("RotateLeft", KeyAction::RotateLeft)
		("RotateRight", KeyAction::RotateRight)
		("PanScreenUp", KeyAction::PanScreenUp)
		("PanScreenDown", KeyAction::PanScreenDown)
		("PanScreenLeft", KeyAction::PanScreenLeft)
		("PanScreenRight", KeyAction::PanScreenRight)
		("PanUpperEdge", KeyAction::PanUpperEdge)
		("PanLowerEdge", KeyAction::PanLowerEdge)
		("PanLeftEdge", KeyAction::PanLeftEdge)
		("PanRightEdge", KeyAction::PanRightEdge)
		;

	const std::map<KeyAction, App::StringID> ActionSids = {
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
		{ App::KeyAction::CopyImage, SIDActionCopyToClipboard },
		{ App::KeyAction::ToggleFullSizeDefaultZoom, SIDActionToggleFullSizeDefaultZoom },
		{ App::KeyAction::RotateLeft, SIDActionRotateLeft },
		{ App::KeyAction::RotateRight, SIDActionRotateRight },
		{ App::KeyAction::PanScreenUp, SIDActionPanScreenUp },
		{ App::KeyAction::PanScreenDown, SIDActionPanScreenDown },
		{ App::KeyAction::PanScreenLeft, SIDActionPanScreenLeft },
		{ App::KeyAction::PanScreenRight, SIDActionPanScreenRight },
		{ App::KeyAction::PanUpperEdge, SIDActionPanEdgeUp },
		{ App::KeyAction::PanLowerEdge, SIDActionPanEdgeDown },
		{ App::KeyAction::PanLeftEdge, SIDActionPanEdgeLeft },
		{ App::KeyAction::PanRightEdge, SIDActionPanEdgeRight }

	};

	const std::map<wxKeyCode, App::StringID> cg_keySids = {
		//0x00
		{ WXK_CANCEL, SIDKeysCancel },
		{ WXK_BACK, SIDKeysBackspace },
		{ WXK_TAB, SIDKeysTab },
		{ WXK_CLEAR, SIDKeysClear },
		{ WXK_RETURN, SIDKeysReturn },
		// 0x10
		{ WXK_SHIFT, SIDKeysShift },
		{ WXK_CONTROL, SIDKeysControl },
		{ WXK_MENU, SIDKeysAlt },
		{ WXK_PAUSE, SIDKeysPause },
		{ WXK_CAPITAL, SIDKeysCapsLock },
		{ WXK_ESCAPE, SIDKeysEsc },
		// 0x20
		{ WXK_SPACE, SIDKeysSpace },
		{ WXK_PAGEUP, SIDKeysPgUp },
		{ WXK_PAGEDOWN, SIDKeysPgDown },
		{ WXK_END, SIDKeysEnd },
		{ WXK_HOME, SIDKeysHome },
		{ WXK_LEFT, SIDKeysArrowLeft },
		{ WXK_UP, SIDKeysArrowUp },
		{ WXK_RIGHT, SIDKeysArrowRight },
		{ WXK_DOWN, SIDKeysArrowDown },
		{ WXK_SELECT, SIDKeysSelect },
		{ WXK_PRINT, SIDKeysPrint },
		{ WXK_EXECUTE, SIDKeysExecute },
		{ WXK_SNAPSHOT, SIDKeysPrintScreen },
		{ WXK_INSERT, SIDKeysInsert },
		{ WXK_DELETE, SIDKeysDelete },
		{ WXK_HELP, SIDKeysHelp },
		// Tons of numbers and characters (automatically handled)

		// 0x50
		{ WXK_WINDOWS_LEFT, SIDKeysWinLeft },
		{ WXK_WINDOWS_RIGHT, SIDKeysWinRight },
		{ WXK_WINDOWS_MENU, SIDKeysApps },

		// 0x60
		{ WXK_NUMPAD0, SIDKeysNum0 },
		{ WXK_NUMPAD1, SIDKeysNum1 },
		{ WXK_NUMPAD2, SIDKeysNum2 },
		{ WXK_NUMPAD3, SIDKeysNum3 },
		{ WXK_NUMPAD4, SIDKeysNum4 },
		{ WXK_NUMPAD5, SIDKeysNum5 },
		{ WXK_NUMPAD6, SIDKeysNum6 },
		{ WXK_NUMPAD7, SIDKeysNum7 },
		{ WXK_NUMPAD8, SIDKeysNum8 },
		{ WXK_NUMPAD9, SIDKeysNum9 },
		{ WXK_MULTIPLY, SIDKeysNumMult },
		{ WXK_ADD, SIDKeysNumPlus },
		{ WXK_SEPARATOR, SIDKeysNumSeparator },
		{ WXK_SUBTRACT, SIDKeysNumMinus },
		{ WXK_DECIMAL, SIDKeysNumDecimal },
		{ WXK_DIVIDE, SIDKeysNumDivide },
		// 0x70
		{ WXK_F1, SIDKeysF1 },
		{ WXK_F2, SIDKeysF2 },
		{ WXK_F3, SIDKeysF3 },
		{ WXK_F4, SIDKeysF4 },
		{ WXK_F5, SIDKeysF5 },
		{ WXK_F6, SIDKeysF6 },
		{ WXK_F7, SIDKeysF7 },
		{ WXK_F8, SIDKeysF8 },
		{ WXK_F9, SIDKeysF9 },
		{ WXK_F10, SIDKeysF10 },
		{ WXK_F11, SIDKeysF11 },
		{ WXK_F12, SIDKeysF12 },
		{ WXK_F13, SIDKeysF13 },
		{ WXK_F14, SIDKeysF14 },
		{ WXK_F15, SIDKeysF15 },
		{ WXK_F16, SIDKeysF16 },
		// 0x80
		{ WXK_F17, SIDKeysF17 },
		{ WXK_F18, SIDKeysF18 },
		{ WXK_F19, SIDKeysF19 },
		{ WXK_F20, SIDKeysF20 },
		{ WXK_F21, SIDKeysF21 },
		{ WXK_F22, SIDKeysF22 },
		{ WXK_F23, SIDKeysF23 },
		{ WXK_F24, SIDKeysF24 },
		// 0x90
		{ WXK_NUMLOCK, SIDKeysNumLock },
		{ WXK_SCROLL, SIDKeysScrollLock },
		// 0xA0
		// TODO: Figure out if it's possible to handle left/right modifiers somewhat simply, otherwise update strings
		{ WXK_SHIFT, SIDKeysShiftLeft },
		{ WXK_CONTROL, SIDKeysControlLeft },
		{ WXK_ALT, SIDKeysAltLeft },
		// TODO: Figure out if these exist in non-windows-land
/*
		{ WXK_BROWSER_BACK, SIDKeysBrowserBack },
		{ WXK_BROWSER_FORWARD, SIDKeysBrowserForward },
		{ WXK_BROWSER_REFRESH, SIDKeysBrowserRefresh },
		{ WXK_BROWSER_STOP, SIDKeysBrowserStop },
		{ WXK_BROWSER_SEARCH, SIDKeysBrowserSearch },
		{ WXK_BROWSER_FAVORITES, SIDKeysBrowserFavorites },
		{ WXK_BROWSER_HOME, SIDKeysBrowserHome },
		{ WXK_VOLUME_MUTE, SIDKeysMediaMute },
		{ WXK_VOLUME_DOWN, SIDKeysMediaVolumeDown },
		{ WXK_VOLUME_UP, SIDKeysMediaVolumeUp },
		// 0xB0
		{ WXK_MEDIA_NEXT_TRACK, SIDKeysMediaNext },
		{ WXK_MEDIA_PREV_TRACK, SIDKeysMediaPrevious },
		{ WXK_MEDIA_STOP, SIDKeysMediaStop },
		{ WXK_MEDIA_PLAY_PAUSE, SIDKeysMediaPlayPause },
		{ WXK_LAUNCH_MAIL, SIDKeysLaunchMail },
		{ WXK_LAUNCH_MEDIA_SELECT, SIDKeysLaunchMedia },
		{ WXK_LAUNCH_APP1, SIDKeysLaunchApp1 },
		{ WXK_LAUNCH_APP2, SIDKeysLaunchApp2 },
		{ WXK_OEM_1, SIDKeysOEM1 },
		{ WXK_OEM_PLUS, SIDKeysPlus },
		{ WXK_OEM_COMMA, SIDKeysComma },
		{ WXK_OEM_MINUS, SIDKeysMinus },
		{ WXK_OEM_PERIOD, SIDKeysPeriod },
		{ WXK_OEM_2, SIDKeysOEM2 },
		// 0xC0
		{ WXK_OEM_3, SIDKeysOEM3 },
		// 0xD0
		{ WXK_OEM_4, SIDKeysOEM4 },
		{ WXK_OEM_5, SIDKeysOEM5 },
		{ WXK_OEM_6, SIDKeysOEM6 },
		{ WXK_OEM_7, SIDKeysOEM7 },
		{ WXK_OEM_8, SIDKeysOEM8 },
		// 0xE0
		{ WXK_OEM_102, SIDKeysOEM102 },
		{ WXK_PROCESSKEY, SIDKeysIMEProcess },
		// 0Xf0
		{ WXK_ATTN, SIDKeysAttn },
		{ WXK_CRSEL, SIDKeysCrSel },
		{ WXK_EXSEL, SIDKeysExSel },
		{ WXK_EREOF, SIDKeysEraseEOF },
		{ WXK_PLAY, SIDKeysMediaPlay },
		{ WXK_ZOOM, SIDKeysZoom },
		{ WXK_PA1, SIDKeysPA1 },*/
		{ WXK_CLEAR, SIDKeysOEMClear }
	};

	App::StringID KeyActionSid(KeyAction action) {
		return ActionSids.find(action)->second;
	}

	std::string KeyActionToIdentifier(KeyAction action) {
		auto it = cg_actionIdentifiers.right.find(action);
		return it->second;
	}

	KeyAction IdentifierToKeyAction(std::string identifier) {
		auto it = cg_actionIdentifiers.left.find(identifier);
		if (it == cg_actionIdentifiers.left.end()) {
			return KeyAction::Undefined;
		}
		return it->second;
	}

	std::wstring GetKeyString(wxKeyCode key) {
		using namespace Intl;
		auto fancyKeySid = cg_keySids.find(key);
		if (fancyKeySid != cg_keySids.end()) {
			return UTF8ToWString(GetString(fancyKeySid->second));
		}

		std::wstring tmp;
		tmp += key;
		return tmp;
	}
}
