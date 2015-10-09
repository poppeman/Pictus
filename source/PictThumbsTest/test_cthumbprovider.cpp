#include <UnitTest++/UnitTest++.h>
#include <thumbcache.h>
#include <Shlwapi.h>
#include "PictThumbs/cthumbprovider.h"
#include "main.h"

SUITE(TestCThumbProvider) {
	TEST(UninitializedGenerateThumb) {
		CPictusThumbnailProvider cptp;
		HBITMAP b;
		WTS_ALPHATYPE a;
		CHECK_EQUAL(E_ILLEGAL_METHOD_CALL, cptp.GetThumbnail(32, &b, &a));
	}

	struct CreateIStreamFixture {
		CreateIStreamFixture():is(0) {}
		void Create(wchar_t* c) {
			auto hr = SHCreateStreamOnFileW((g_datapath + L"\\" + std::wstring(c)).c_str(), STGM_READ | STGM_SHARE_DENY_WRITE, &is);
			if (FAILED(hr)) {
				DO_THROW(Err::CriticalError, L"Failed creating stream. Error code: " + ToWString(hr));
			}
		}

		IStream* is;
		CPictusThumbnailProvider cptp;

		HBITMAP b;
		WTS_ALPHATYPE a;
	};

	TEST_FIXTURE(CreateIStreamFixture, GenerateNullParams) {
		Create(L"simple.tga");
		cptp.Initialize(is, 0);
		CHECK_EQUAL(E_POINTER, cptp.GetThumbnail(32, 0, 0));
		CHECK_EQUAL(E_INVALIDARG, cptp.GetThumbnail(0, 0, 0));
	}


	/*
	TEST_FIXTURE(CreateIStreamFixture, SimpleGenerate) {
		Create(L"bw.tga");
		cptp.Initialize(is, 0);
		CHECK_EQUAL(S_OK, cptp.GetThumbnail(32, &b, &a));
		CHECK_EQUAL(WTSAT_RGB, a);
		CHECK(b);
	}*/

/*	TEST_FIXTURE(CreateIStreamFixture, TiffOffL16) {
		Create(L"Codecs\\TIFF\\quad-jpeg.tif");
		cptp.Initialize(is, 0);
		CHECK_EQUAL(S_OK, cptp.GetThumbnail(32, &b, &a));
		CHECK_EQUAL(WTSAT_RGB, a);
		CHECK(b);
	}*/

/*	TEST_FIXTURE(CreateIStreamFixture, TgaNoAlpha) {
		Create(L"Codecs\\TIFF\\quad-jpeg.tga");
		cptp.Initialize(is, 0);
		CHECK_EQUAL(S_OK, cptp.GetThumbnail(32, &b, &a));
		CHECK_EQUAL(WTSAT_RGB, a);
		CHECK(b);
		DeleteObject(b);
	}*/
}
