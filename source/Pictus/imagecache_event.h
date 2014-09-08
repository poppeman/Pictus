#ifndef IMAGECACHE_EVENT_H
#define IMAGECACHE_EVENT_H

#include "illa/image.h"

namespace Img {
	class MessageReceiver {
	public:
		enum LoadMessage {
			LoadHeader,
			LoadAllocated,
			LoadDone,
			LoadErrorImage,
			LoadErrorCritical
		};

	public:
		virtual void OnLoadMessage(LoadMessage s, Img::Image* image, const std::wstring& desc)=0;
	};
}

#endif
