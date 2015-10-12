#include "imagecache_workrequest.h"

namespace Img {
	WorkRequest::WorkRequest(Image* i, std::string s) :
		image(i),
		filename(s)
	{}

	bool operator==(const WorkRequest& a, const WorkRequest& b)
	{
		return (a.image == b.image) && (a.filename == b.filename);
	}

	bool operator==(const WorkRequest& a, const Image* b)
	{
		return (a.image == b);
	}

	bool operator<(const WorkRequest& a, const WorkRequest& b)
	{
		return a.image < b.image;
	}
}
