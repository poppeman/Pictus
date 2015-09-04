#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "illa/types.h"

namespace App {
#include "lang_enum.h"

	enum ResizeBehaviour {
		ResizeEnlargeOrReduce = 0,
		ResizeEnlargeOnly,
		ResizeReduceOnly
	};

	enum ZoomMode {
		ZoomFree = 0,
		ZoomFitImage,
		ZoomFullSize,
	};

	enum ViewerAppMessage {
		ViewerImageLoadEvent = 1,
//		ViewerSettingsChanged,
		ViewerListUpdate,
	};

	enum class KeyAction {
		Undefined = 0,
		NextImage = 1,
		NextSkipImage = 2,
		PreviousImage = 3,
		PreviousSkipImage = 4,
		RenameFile = 5,
		RandomImage = 6,
		ZoomIn = 7,
		ZoomOut = 8,
		ZoomDefault = 9,
		ZoomFull = 10,
		ZoomFree = 11,
		PanUp = 12,
		PanDown = 13,
		PanLeft = 14,
		PanRight = 15,
		FirstImage = 16,
		LastImage = 17,
		RecycleCurrentFile = 18,  // Send file to recycle bin OR delete, with out without confirmation dialog, depending on OS settings.
		DeleteCurrentFile = 19,   // Immediately deletes the file, bypassing any recycle bin.
		RemoveCurrentImage = 19,  // Removes image from rotation, does NOT touch the actual file.
		ToggleFullscreen = 20,
		CloseApplication = 21,
		OpenSettings = 22
	};

	struct KeyboardPress {
		wchar_t Key;
		bool Alt;
		bool Shift;
		bool Ctrl;
	};

	StringID KeyActionSid(KeyAction action);

	std::string KeyActionToIdentifier(KeyAction action);
	KeyAction IdentifierToKeyAction(std::string identifier);

	std::wstring GetKeyString(wchar_t key);
}

#endif
