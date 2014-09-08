
#ifndef IMAGECACHE_SHARED_H
#define IMAGECACHE_SHARED_H

#include "illa/image.h"

namespace Img {
	struct WorkRequest {
		Image* image;
		std::wstring filename;

		WorkRequest(Image* i, std::wstring s):image(i), filename(s) {}
	};

	inline bool operator==(const WorkRequest& a, const WorkRequest& b) {
		return (a.image == b.image) && (a.filename == b.filename);
	}

	inline bool operator==(const WorkRequest& a, const Image* b) {
		return (a.image == b);
	}

	inline bool operator <(const WorkRequest& a, const WorkRequest& b) {
		return a.image < b.image;
	}

	typedef std::vector<WorkRequest>	ImageList;
}

#endif // IMAGECACHE_SHARED_H
