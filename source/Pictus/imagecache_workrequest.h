#ifndef PICTUS_IMAGECACHE_SHARED_H
#define PICTUS_IMAGECACHE_SHARED_H

#include "illa/image.h"

namespace Img {
	struct WorkRequest {
		Image* image;
		std::string filename;

		WorkRequest(Image* i, std::string s);
	};

	bool operator==(const WorkRequest& a, const WorkRequest& b);

	bool operator==(const WorkRequest& a, const Image* b);

	bool operator <(const WorkRequest& a, const WorkRequest& b);

	typedef std::vector<WorkRequest>	ImageList;
}

#endif
