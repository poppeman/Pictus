#ifndef APPREG_H
#define APPREG_H

#include "orz/intl.h"
#include "illa/types.h"
#include "app_types.h"

namespace App {
	enum class ResizePositionMethod {
		PositionToScreen,
		PositionToCurrent,
		PositionNothing
	};

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
		MouseFullscreenPreviewNoClick,
		MouseFullscreenPreviewClick,
		MouseUndefined,
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
	};

	struct KeyboardBinding {
		wchar_t Key;
		bool Alt;
		bool Shift;
		bool Ctrl;
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

std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<wchar_t>& in, const App::ResizePositionMethod& fmt);

#endif