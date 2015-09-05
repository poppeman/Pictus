#ifndef PICTUS_ZOOM_MODE_H
#define PICTUS_ZOOM_MODE_H

namespace App {
	enum ZoomMode {
		ZoomFree = 0,
		ZoomFitImage,
		ZoomFullSize,
	};

	ZoomMode IdentifierToZoomMode(std::string identifier);
	std::string ZoomModeToIdentifier(ZoomMode mode);
}


#endif
