#ifndef APPREG_H
#define APPREG_H

#include "orz/intl.h"

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
	struct DWordNode {
		const wchar_t* Name;
		int Value;
	};

	namespace Keys {
#include "keys_dword.inc"
	}
}

std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<wchar_t>& in, const App::ResizePositionMethod& fmt);

#endif