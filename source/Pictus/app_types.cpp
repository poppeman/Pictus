#include "StdAfx.h"
#include "app_types.h"

namespace App {
	App::StringID KeyActionSid(KeyAction action) {
		switch (action) {
		case App::KeyAction::NextImage:
			return SIDActionNextImage;;
		case App::KeyAction::PreviousImage:
			return SIDActionPrevImage;
		default:
			return SIDInvalid;
		}
	}
}
