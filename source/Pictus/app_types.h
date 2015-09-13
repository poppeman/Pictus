#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "illa/types.h"
#include "resize_behavior.h"

namespace App {
#include "lang_enum.h"
	enum ViewerAppMessage {
		ViewerImageLoadEvent = 1,
//		ViewerSettingsChanged,
		ViewerListUpdate,
	};

	enum class KeyAction {
		Undefined,
		NextImage,
		NextSkipImage,
		PreviousImage,
		PreviousSkipImage,
		RenameFile,
		RandomImage,
		ZoomIn,
		ZoomOut,
		ZoomDefault,
		ZoomFull,
		ZoomFree,
		PanUp,
		PanDown,
		PanLeft,
		PanRight,
		FirstImage,
		LastImage,
		RecycleCurrentFile,  // Send file to recycle bin OR delete, with out without confirmation dialog, depending on OS settings.
		DeleteCurrentFile,   // Immediately deletes the file, bypassing any recycle bin.
		RemoveCurrentImage,  // Removes image from rotation, does NOT touch the actual file.
		ToggleFullscreen,
		CloseApplication,
		OpenSettings,
		CopyImage,
	};

	struct KeyboardPress {
		wchar_t Key;
		bool Alt;
		bool Shift;
		bool Ctrl;
	};

	extern const std::map<KeyAction, App::StringID> ActionSids;

	StringID KeyActionSid(KeyAction action);

	std::string KeyActionToIdentifier(KeyAction action);
	KeyAction IdentifierToKeyAction(std::string identifier);

	std::wstring GetKeyString(wchar_t key);
}

#endif
