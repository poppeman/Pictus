#ifndef IC_WORKPACKAGE_H
#define IC_WORKPACKAGE_H

#include "ic_imageloader.h"

namespace Img {
	class Image;

	enum WorkOperation {
		WOUndefined,
		WOLoadImage,
		WODone,
	};

	class WorkPackage {
	public:
		WorkOperation Operation() const;
		bool FastPass() const;
		size_t MemoryUsage() const;
		ImageLoader::Ptr Loader;

		explicit WorkPackage(ImageLoader::Ptr l):Loader(l) {}
		explicit WorkPackage() {}
		~WorkPackage();

		bool operator==(const Image* b);
	};
}

#endif
