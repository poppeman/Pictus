#ifndef IMAGECACHE_QUEUE_H
#define IMAGECACHE_QUEUE_H

#include "ic_imageloader.h"
#include "imagecache_workrequest.h"

namespace Img {
	class ImageQueue {
	public:
		void CacheImage(const ImageList& newImagesToProcess);
		void RemoveImage(Image* image);

		ImageList GetImageList();

		void Reset();

		typedef std::shared_ptr<ImageQueue> Ptr;

	private:
		std::mutex m_mxPackageList;
		ImageList m_workPackages;
	};
}

#endif
