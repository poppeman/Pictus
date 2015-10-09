#include <UnitTest++/UnitTest++.h>
#include "main.h"
#include "Pictus/ic_queue.h"


SUITE(ImageCacherQueue) {
	using namespace Img;
/*	
	bool operator==(const Img::WorkPackage& a, const Img::WorkPackage& b) {
		return a.Loader == b.Loader && a.Operation() == b.Operation();
	}

	struct FixtureBwImage {
		FixtureBwImage():a(bwFile), bwFile(g_datapath + L"\\bw.tga") {
			ImageList l;
			l.push_back(&a);
			iq.CacheImage(l);
		}

		std::wstring bwFile;
		ImageQueue iq;
		Image a;
		WorkPackage p;
	};

	struct FixtureTwoImages {
		FixtureTwoImages():a(g_datapath + L"\\bw.tga"), b(g_datapath + L"\\bw2.tga") {
			ImageList l;
			l.push_back(&a);
			l.push_back(&b);
			iq.CacheImage(l);
		}

		std::wstring bwFile;
		ImageQueue iq;
		Image a;
		Image b;
		WorkPackage p;
	};

	TEST(DefaultState) {
		ImageQueue iq;
		WorkPackage p;
		CHECK(!iq.FetchWorkPackage(&p, false));
	}

	TEST_FIXTURE(FixtureBwImage, SimpleQueue) {
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK_EQUAL(p.Operation(), WOLoadImage);
		CHECK(p.Loader); if(!p.Loader) return;
		CHECK(p.Loader->GetImage() != 0); if(!p.Loader->GetImage()) return;
		CHECK_EQUAL(bwFile, p.Loader->GetImage()->FileName());
		CHECK_EQUAL(p.Loader->GetState(), LoadStateUnprocessed);
	}

	TEST_FIXTURE(FixtureBwImage, TestNoDoubleFetch) {
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK_THROW(iq.FetchWorkPackage(&p, false), Err::InvalidCall);
	}

	TEST_FIXTURE(FixtureBwImage, TestNoImmediateReturn) {
		CHECK_THROW(iq.ReturnPackage(), Err::InvalidCall);
	}

	TEST_FIXTURE(FixtureBwImage, TestNoDoubleReturn) {
		CHECK(iq.FetchWorkPackage(&p, false));
		iq.ReturnPackage();
		CHECK_THROW(iq.ReturnPackage(), Err::InvalidCall);
	}

	TEST_FIXTURE(FixtureBwImage, TestUntouchedPackage) {
		CHECK(iq.FetchWorkPackage(&p, false));
		iq.ReturnPackage();
		WorkPackage q;
		CHECK(iq.FetchWorkPackage(&q, false));
		CHECK(p == q);
		CHECK_EQUAL(WOLoadImage, q.Operation());
		CHECK_EQUAL(LoadStateUnprocessed, q.Loader->GetState());
		p.Loader->LoadHeader();
		iq.ReturnPackage();
		CHECK(iq.FetchWorkPackage(&p, false));
		iq.ReturnPackage();
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK_EQUAL(WOLoadImage, p.Operation());
	}

	TEST_FIXTURE(FixtureTwoImages, TestImageLoadProgress) {
		CHECK(iq.FetchWorkPackage(&p, false));
		p.Loader->LoadHeader();
		iq.ReturnPackage();
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK_EQUAL(Img::WOLoadImage, p.Operation());

		p.Loader->Allocate();
		p.Loader->LoadImageData();
		iq.ReturnPackage();
		WorkPackage q;
		CHECK(iq.FetchWorkPackage(&q, false));
		CHECK(!(p == q));
	}


	TEST_FIXTURE(FixtureTwoImages, TestRemoveImage) {
		iq.RemoveImage(&a);
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK(p.Loader->GetImage() == &b);
	}


	TEST_FIXTURE(FixtureTwoImages, TestRemoveSecondImage) {
		iq.RemoveImage(&b);
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK(p.Loader->GetImage() == &a);
	}


	TEST_FIXTURE(FixtureTwoImages, TestRemoveImageWhileProcessing) {
		iq.FetchWorkPackage(&p, false);
		iq.RemoveImage(&a);
		iq.ReturnPackage();
		CHECK(iq.FetchWorkPackage(&p, false));
		CHECK(p.Loader->GetImage() == &b);
	}*/
}
