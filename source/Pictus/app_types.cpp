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
		{ App::KeyAction::RotateRight, SIDActionRotateRight }
	};

	const std::map<wchar_t, App::StringID> cg_keySids = {
		//0x00
		{ WXK_LBUTTON, SIDKeysMouseLeft },
		{ WXK_RBUTTON, SIDKeysMouseRight},
		{ WXK_CANCEL, SIDKeysCancel },
		{ WXK_MBUTTON, SIDKeysMouseMiddle},
		{ WXK_XBUTTON1, SIDKeysMouseX1 },
		{ WXK_XBUTTON2, SIDKeysMouseX2 },
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
		{ WXK_KANA, SIDKeysIMEKana },
		{ WXK_JUNJA, SIDKeysIMEJunja },
		{ WXK_FINAL, SIDKeysIMEFinal },
		{ WXK_KANJI, SIDKeysIMEKanji },
		{ WXK_ESCAPE, SIDKeysEsc },
		{ WXK_CONVERT, SIDKeysIMEConvert },
		{ WXK_NONCONVERT, SIDKeysIMENoConvert },
		{ WXK_ACCEPT, SIDKeysIMEAccept },
		{ WXK_MODECHANGE, SIDKeysIMEModeChange },
		// 0x20
		{ WXK_SPACE, SIDKeysSpace },
		{ WXK_PRIOR, SIDKeysPgUp },
		{ WXK_NEXT, SIDKeysPgDown },
		{ WXK_END, SIDKeysEnd },
		{ WXK_HOME, SIDKeysHome },
		{ WXK_LEFT, SIDKeysArrowLeft },
		{ WXK_UP, SIDKeysArrowUp },
		{ WXK_RIGHT, SIDKeysArrowRight },
		{ WXR_DOWN, SIDKeysArrowDown },
		{ WXR_SELECT, SIDKeysSelect },
		{ WXR_PRINT, SIDKeysPrint },
		{ WXR_EXECUTE, SIDKeysExecute },
		{ WXR_SNAPSHOT, SIDKeysPrintScreen },
		{ WXR_INSERT, SIDKeysInsert },
		{ WXR_DELETE, SIDKeysDelete },
		{ WXR_HELP, SIDKeysHelp },
		// Tons of numbers and characters (automatically handled)

		// 0x50
		{ WXR_LWIN, SIDKeysWinLeft },
		{ WXR_RWIN, SIDKeysWinRight },
		{ WXR_APPS, SIDKeysApps },
		{ WXR_SLEEP, SIDKeysSleep },  // I hope nobody tries using this...
		// 0x60
		{ WXR_NUMPAD0, SIDKeysNum0 },
		{ WXR_NUMPAD1, SIDKeysNum1 },
		{ WXR_NUMPAD2, SIDKeysNum2 },
		{ WXR_NUMPAD3, SIDKeysNum3 },
		{ WXR_NUMPAD4, SIDKeysNum4 },
		{ WXR_NUMPAD5, SIDKeysNum5 },
		{ WXR_NUMPAD6, SIDKeysNum6 },
		{ WXR_NUMPAD7, SIDKeysNum7 },
		{ WXR_NUMPAD8, SIDKeysNum8 },
		{ WXR_NUMPAD9, SIDKeysNum9 },
		{ WXR_MULTIPLY, SIDKeysNumMult },
		{ WXR_ADD, SIDKeysNumPlus },
		{ WXR_SEPARATOR, SIDKeysNumSeparator },
		{ WXR_SUBTRACT, SIDKeysNumMinus },
		{ WXR_DECIMAL, SIDKeysNumDecimal },
		{ WXR_DIVIDE, SIDKeysNumDivide },
		// 0x70
		{ WXR_F1, SIDKeysF1 },
		{ WXR_F2, SIDKeysF2 },
		{ WXR_F3, SIDKeysF3 },
		{ WXR_F4, SIDKeysF4 },
		{ WXR_F5, SIDKeysF5 },
		{ WXR_F6, SIDKeysF6 },
		{ WXR_F7, SIDKeysF7 },
		{ WXR_F8, SIDKeysF8 },
		{ WXR_F9, SIDKeysF9 },
		{ WXR_F10, SIDKeysF10 },
		{ WXR_F11, SIDKeysF11 },
		{ WXR_F12, SIDKeysF12 },
		{ WXR_F13, SIDKeysF13 },
		{ WXR_F14, SIDKeysF14 },
		{ WXR_F15, SIDKeysF15 },
		{ WXR_F16, SIDKeysF16 },
		// 0x80
		{ WXR_F17, SIDKeysF17 },
		{ WXR_F18, SIDKeysF18 },
		{ WXR_F19, SIDKeysF19 },
		{ WXR_F20, SIDKeysF20 },
		{ WXR_F21, SIDKeysF21 },
		{ WXR_F22, SIDKeysF22 },
		{ WXR_F23, SIDKeysF23 },
		{ WXR_F24, SIDKeysF24 },
		// 0x90
		{ WXR_NUMLOCK, SIDKeysNumLock },
		{ WXR_SCROLL, SIDKeysScrollLock },
		// 0xA0
		{ WXR_LSHIFT, SIDKeysShiftLeft },
		{ WXR_RSHIFT, SIDKeysShiftRight },
		{ WXR_LCONTROL, SIDKeysControlLeft },
		{ WXR_RCONTROL, SIDKeysControlRight },
		{ WXR_LMENU, SIDKeysAltLeft },
		{ WXR_RMENU, SIDKeysAltRight },
		{ WXR_BROWSER_BACK, SIDKeysBrowserBack },
		{ WXR_BROWSER_FORWARD, SIDKeysBrowserForward },
		{ WXR_BROWSER_REFRESH, SIDKeysBrowserRefresh },
		{ WXR_BROWSER_STOP, SIDKeysBrowserStop },
		{ WXR_BROWSER_SEARCH, SIDKeysBrowserSearch },
		{ WXR_BROWSER_FAVORITES, SIDKeysBrowserFavorites },
		{ WXR_BROWSER_HOME, SIDKeysBrowserHome },
		{ WXR_VOLUME_MUTE, SIDKeysMediaMute },
		{ WXR_VOLUME_DOWN, SIDKeysMediaVolumeDown },
		{ WXR_VOLUME_UP, SIDKeysMediaVolumeUp },
		// 0xB0
		{ WXR_MEDIA_NEXT_TRACK, SIDKeysMediaNext },
		{ WXR_MEDIA_PREV_TRACK, SIDKeysMediaPrevious },
		{ WXR_MEDIA_STOP, SIDKeysMediaStop },
		{ WXR_MEDIA_PLAY_PAUSE, SIDKeysMediaPlayPause },
		{ WXR_LAUNCH_MAIL, SIDKeysLaunchMail },
		{ WXR_LAUNCH_MEDIA_SELECT, SIDKeysLaunchMedia },
		{ WXR_LAUNCH_APP1, SIDKeysLaunchApp1 },
		{ WXR_LAUNCH_APP2, SIDKeysLaunchApp2 },
		{ WXR_OEM_1, SIDKeysOEM1 },
		{ WXR_OEM_PLUS, SIDKeysPlus },
		{ WXR_OEM_COMMA, SIDKeysComma },
		{ WXR_OEM_MINUS, SIDKeysMinus },
		{ WXR_OEM_PERIOD, SIDKeysPeriod },
		{ WXR_OEM_2, SIDKeysOEM2 },
		// 0xC0
		{ WXR_OEM_3, SIDKeysOEM3 },
		// 0xD0
		{ WXR_OEM_4, SIDKeysOEM4 },
		{ WXR_OEM_5, SIDKeysOEM5 },
		{ WXR_OEM_6, SIDKeysOEM6 },
		{ WXR_OEM_7, SIDKeysOEM7 },
		{ WXR_OEM_8, SIDKeysOEM8 },
		// 0xE0
		{ WXR_OEM_102, SIDKeysOEM102 },
		{ WXR_PROCESSKEY, SIDKeysIMEProcess },
		// 0Xf0
		{ WXR_ATTN, SIDKeysAttn },
		{ WXR_CRSEL, SIDKeysCrSel },
		{ WXR_EXSEL, SIDKeysExSel },
		{ WXR_EREOF, SIDKeysEraseEOF },
		{ WXR_PLAY, SIDKeysMediaPlay },
		{ WXR_ZOOM, SIDKeysZoom },
		{ WXR_PA1, SIDKeysPA1 },
		{ WXR_CLEAR, SIDKeysOEMClear }
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
			return UTF8ToWString(GetString(fancyKeySid->second));
		}


		std::wstring tmp;
		tmp += key;
		return tmp;
	}
}
