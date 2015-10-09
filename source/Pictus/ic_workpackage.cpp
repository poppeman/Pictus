#include "ic_workpackage.h"
#include "ic_imageloader.h"
#include "illa/image.h"

namespace Img {
	Img::WorkOperation Img::WorkPackage::Operation() const {
		if (Loader == 0) DO_THROW(Err::InvalidCall, L"Loader not set.");

		if(Loader->GetState() < LoadStateDone) return WOLoadImage;
		return WODone;
	}

	bool WorkPackage::operator==( const Image* b ) {
		return (Loader->GetImage() == b);
	}

	WorkPackage::~WorkPackage() {
		//delete Loader;
	}

	size_t WorkPackage::MemoryUsage() const {
		if (Loader == 0) DO_THROW(Err::InvalidCall, L"Loader not set");
		if (Loader->GetState() < LoadStateHeader) DO_THROW(Err::InvalidCall, L"Image not in valid state.");
		return Loader->MemoryRequirements();
	}

	bool WorkPackage::FastPass() const {
		return !Loader->FastPassFinished();
	}
}
