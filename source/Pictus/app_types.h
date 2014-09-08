#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "illa/types.h"

namespace App {
#include "lang_sids.inc"

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

};

#endif
