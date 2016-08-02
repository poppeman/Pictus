#ifndef APPREG_H
#define APPREG_H

#include "orz/intl.h"
#include "illa/types.h"
#include "app_types.h"
#include "resize_position_method.h"
#include "zoom_mode.h"

namespace App {
	// TODO: Convert to enum class
	enum MouseAction {
		MouseDisable,
		MousePan,
		MouseContext,
		MouseToggleFullSizeDefaultZoom,
		MouseNextImage,
		MousePrevImage,
		MouseZoomIn,
		MouseZoomOut,
		MouseFullscreen,
		MouseFullscreenPreviewNoClick,   // Not used
		MouseFullscreenPreviewClick,     // Not used
		MouseRotateLeft,
		MouseRotateRight,
		MouseUndefined
	};
}

namespace Reg {
	struct RenderSettings {
		Filter::Mode MagFilter;
		Filter::Mode MinFilter;
		Img::Color BackgroundColor;
	};

	struct ViewSettings {
		bool ResetZoom;
		App::ResizePositionMethod ResizePositionMethod;
		bool ResizeWindow;
		bool ResetPan;
		Intl::Language Language;
		int32_t WindowSizeWidth;
		int32_t WindowSizeHeight;
		int32_t WindowPosX;
		int32_t WindowPosY;
		int32_t WindowAnchorTLX;
		int32_t WindowAnchorTLY;
		int32_t WindowAnchorCenterX;
		int32_t WindowAnchorCenterY;
		bool Maximized;
		App::ZoomMode DefaultZoomMode;
		App::ResizeBehaviour ResizeBehaviour;
		bool ShowStatusBar;
		bool AlwaysOnTop;
		bool BrowseWrapAround;
		bool MultipleInstances;
	};

	struct MouseSettings {
		App::MouseAction OnMouseLeft;
		App::MouseAction OnMouseMiddle;
		App::MouseAction OnMouseRight;
		App::MouseAction OnMouseLeftDbl;
		App::MouseAction OnMouseMiddleDbl;
		App::MouseAction OnMouseRightDbl;
		App::MouseAction OnMouseWheelUp;
		App::MouseAction OnMouseWheelDown;
		App::MouseAction OnMouseExtra1;
		App::MouseAction OnMouseExtra2;
		App::MouseAction OnMouseExtra1Dbl;
		App::MouseAction OnMouseExtra2Dbl;
	};

	struct KeyboardBinding {
		App::KeyboardPress Key;
		App::KeyAction Action;
	};

	struct KeyboardSettings {
		std::vector<KeyboardBinding> Bindings;
	};

	struct CacheSettings {
		bool DoAutoMemoryLimit;
		size_t ManualMemoryLimit;
	};

	struct Settings {
		RenderSettings Render;
		ViewSettings View;
		MouseSettings Mouse;
		CacheSettings Cache;
		KeyboardSettings Keyboard;
	};
}


#endif
