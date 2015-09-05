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
		("CopyImage", KeyAction::CopyImage)
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
		{ App::KeyAction::CopyImage, SIDActionCopyToClipboard }
	};

	const std::map<wchar_t, App::StringID> cg_keySids = {
		//0x00
		{ VK_LBUTTON, SIDKeysMouseLeft },
		{ VK_RBUTTON, SIDKeysMouseRight},
		{ VK_CANCEL, SIDKeysCancel },
		{ VK_MBUTTON, SIDKeysMouseMiddle},
		{ VK_XBUTTON1, SIDKeysMouseX1 },
		{ VK_XBUTTON2, SIDKeysMouseX2 },
		{ VK_BACK, SIDKeysBackspace },
		{ VK_TAB, SIDKeysTab },
		{ VK_CLEAR, SIDKeysClear },
		{ VK_RETURN, SIDKeysReturn },
		// 0x10
		{ VK_SHIFT, SIDKeysShift },
		{ VK_CONTROL, SIDKeysControl },
		{ VK_MENU, SIDKeysAlt },
		{ VK_PAUSE, SIDKeysPause },
		{ VK_CAPITAL, SIDKeysCapsLock },
		{ VK_KANA, SIDKeysIMEKana },
		{ VK_JUNJA, SIDKeysIMEJunja },
		{ VK_FINAL, SIDKeysIMEFinal },
		{ VK_KANJI, SIDKeysIMEKanji },
		{ VK_ESCAPE, SIDKeysEsc },
		{ VK_CONVERT, SIDKeysIMEConvert },
		{ VK_NONCONVERT, SIDKeysIMENoConvert },
		{ VK_ACCEPT, SIDKeysIMEAccept },
		{ VK_MODECHANGE, SIDKeysIMEModeChange },
		// 0x20
		{ VK_SPACE, SIDKeysSpace },
		{ VK_PRIOR, SIDKeysPgUp },
		{ VK_NEXT, SIDKeysPgDown },
		{ VK_END, SIDKeysEnd },
		{ VK_HOME, SIDKeysHome },
		{ VK_LEFT, SIDKeysArrowLeft },
		{ VK_UP, SIDKeysArrowUp },
		{ VK_RIGHT, SIDKeysArrowRight },
		{ VK_DOWN, SIDKeysArrowDown },
		{ VK_SELECT, SIDKeysSelect },
		{ VK_PRINT, SIDKeysPrint },
		{ VK_EXECUTE, SIDKeysExecute },
		{ VK_SNAPSHOT, SIDKeysPrintScreen },
		{ VK_INSERT, SIDKeysInsert },
		{ VK_DELETE, SIDKeysDelete },
		{ VK_HELP, SIDKeysHelp },
		// Tons of numbers and characters (automatically handled)

		// 0x50
		{ VK_LWIN, SIDKeysWinLeft },
		{ VK_RWIN, SIDKeysWinRight },
		{ VK_APPS, SIDKeysApps },
		{ VK_SLEEP, SIDKeysSleep },  // I hope nobody tries using this...
		// 0x60
		{ VK_NUMPAD0, SIDKeysNum0 },
		{ VK_NUMPAD1, SIDKeysNum1 },
		{ VK_NUMPAD2, SIDKeysNum2 },
		{ VK_NUMPAD3, SIDKeysNum3 },
		{ VK_NUMPAD4, SIDKeysNum4 },
		{ VK_NUMPAD5, SIDKeysNum5 },
		{ VK_NUMPAD6, SIDKeysNum6 },
		{ VK_NUMPAD7, SIDKeysNum7 },
		{ VK_NUMPAD8, SIDKeysNum8 },
		{ VK_NUMPAD9, SIDKeysNum9 },
		{ VK_MULTIPLY, SIDKeysNumMult },
		{ VK_ADD, SIDKeysNumPlus },
		{ VK_SEPARATOR, SIDKeysNumSeparator },
		{ VK_SUBTRACT, SIDKeysNumMinus },
		{ VK_DECIMAL, SIDKeysNumDecimal },
		{ VK_DIVIDE, SIDKeysNumDivide },
		// 0x70
		{ VK_F1, SIDKeysF1 },
		{ VK_F2, SIDKeysF2 },
		{ VK_F3, SIDKeysF3 },
		{ VK_F4, SIDKeysF4 },
		{ VK_F5, SIDKeysF5 },
		{ VK_F6, SIDKeysF6 },
		{ VK_F7, SIDKeysF7 },
		{ VK_F8, SIDKeysF8 },
		{ VK_F9, SIDKeysF9 },
		{ VK_F10, SIDKeysF10 },
		{ VK_F11, SIDKeysF11 },
		{ VK_F12, SIDKeysF12 },
		{ VK_F13, SIDKeysF13 },
		{ VK_F14, SIDKeysF14 },
		{ VK_F15, SIDKeysF15 },
		{ VK_F16, SIDKeysF16 },
		// 0x80
		{ VK_F17, SIDKeysF17 },
		{ VK_F18, SIDKeysF18 },
		{ VK_F19, SIDKeysF19 },
		{ VK_F20, SIDKeysF20 },
		{ VK_F21, SIDKeysF21 },
		{ VK_F22, SIDKeysF22 },
		{ VK_F23, SIDKeysF23 },
		{ VK_F24, SIDKeysF24 },
		// 0x90
		{ VK_NUMLOCK, SIDKeysNumLock },
		{ VK_SCROLL, SIDKeysScrollLock },
		// 0xA0
		{ VK_LSHIFT, SIDKeysShiftLeft },
		{ VK_RSHIFT, SIDKeysShiftRight },
		{ VK_LCONTROL, SIDKeysControlLeft },
		{ VK_RCONTROL, SIDKeysControlRight },
		{ VK_LMENU, SIDKeysAltLeft },
		{ VK_RMENU, SIDKeysAltRight },
		{ VK_BROWSER_BACK, SIDKeysBrowserBack },
		{ VK_BROWSER_FORWARD, SIDKeysBrowserForward },
		{ VK_BROWSER_REFRESH, SIDKeysBrowserRefresh },
		{ VK_BROWSER_STOP, SIDKeysBrowserStop },
		{ VK_BROWSER_SEARCH, SIDKeysBrowserSearch },
		{ VK_BROWSER_FAVORITES, SIDKeysBrowserFavorites },
		{ VK_BROWSER_HOME, SIDKeysBrowserHome },
		{ VK_VOLUME_MUTE, SIDKeysMediaMute },
		{ VK_VOLUME_DOWN, SIDKeysMediaVolumeDown },
		{ VK_VOLUME_UP, SIDKeysMediaVolumeUp },
		// 0xB0
		{ VK_MEDIA_NEXT_TRACK, SIDKeysMediaNext },
		{ VK_MEDIA_PREV_TRACK, SIDKeysMediaPrevious },
		{ VK_MEDIA_STOP, SIDKeysMediaStop },
		{ VK_MEDIA_PLAY_PAUSE, SIDKeysMediaPlayPause },
		{ VK_LAUNCH_MAIL, SIDKeysLaunchMail },
		{ VK_LAUNCH_MEDIA_SELECT, SIDKeysLaunchMedia },
		{ VK_LAUNCH_APP1, SIDKeysLaunchApp1 },
		{ VK_LAUNCH_APP2, SIDKeysLaunchApp2 },
		{ VK_OEM_1, SIDKeysOEM1 },
		{ VK_OEM_PLUS, SIDKeysPlus },
		{ VK_OEM_COMMA, SIDKeysComma },
		{ VK_OEM_MINUS, SIDKeysMinus },
		{ VK_OEM_PERIOD, SIDKeysPeriod },
		{ VK_OEM_2, SIDKeysOEM2 },
		// 0xC0
		{ VK_OEM_3, SIDKeysOEM3 },
		// 0xD0
		{ VK_OEM_4, SIDKeysOEM4 },
		{ VK_OEM_5, SIDKeysOEM5 },
		{ VK_OEM_6, SIDKeysOEM6 },
		{ VK_OEM_7, SIDKeysOEM7 },
		{ VK_OEM_8, SIDKeysOEM8 },
		// 0xE0
		{ VK_OEM_102, SIDKeysOEM102 },
		{ VK_PROCESSKEY, SIDKeysIMEProcess },
		// 0Xf0
		{ VK_ATTN, SIDKeysAttn },
		{ VK_CRSEL, SIDKeysCrSel },
		{ VK_EXSEL, SIDKeysExSel },
		{ VK_EREOF, SIDKeysEraseEOF },
		{ VK_PLAY, SIDKeysMediaPlay },
		{ VK_ZOOM, SIDKeysZoom },
		{ VK_PA1, SIDKeysPA1 },
		{ VK_CLEAR, SIDKeysOEMClear }
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
		return it->second;
	}

	std::wstring GetKeyString(wchar_t key) {
		using namespace Intl;
		auto fancyKeySid = cg_keySids.find(key);
		if (fancyKeySid != cg_keySids.end()) {
			return GetWString(fancyKeySid->second);
		}


		std::wstring tmp;
		tmp += key;
		return tmp;
	}
}
