#ifndef APPREG_H
#define APPREG_H

#include "orz/intl.h"

namespace App {
	enum class ResizePositionMethod {
		PositionToScreen,
		PositionToCurrent,
		PositionNothing
	};

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
	struct DWordNode {
		const wchar_t* Name;
		int Value;
	};

	namespace Keys {
#include "keys_dword.inc"
	};
}

inline std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<wchar_t>& in, const App::ResizePositionMethod& fmt) {
	switch (fmt) {
	case App::ResizePositionMethod::PositionToScreen:
		in << L"PositionToScreen";
		break;
	case App::ResizePositionMethod::PositionToCurrent:
		in << L"PositionToCurrent";
		break;
	case App::ResizePositionMethod::PositionNothing:
		in << L"PositionNothing";
		break;
	default:
		in << L"Unknown(" << static_cast<int>(fmt) << L")";
		break;
	}
	return in;
}


#endif