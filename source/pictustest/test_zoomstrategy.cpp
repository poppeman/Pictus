#include "StdAfx.h"
#include "Pictus/zoomstrategy.h"

SUITE(SuiteZoomStrategy) {
	using namespace App;
	using namespace Geom;

	struct SimpleReduceFixture {
		ZoomStrategy z;
		ZoomStrategy::Result r;
		SimpleReduceFixture() {
			z.ResizeBehaviour(ResizeReduceOnly);
			z.ZoomMode(ZoomFitImage);
		}
	};

	TEST_FIXTURE(SimpleReduceFixture, SimpleReduceOnly) {
		r = z.CalculateViewAreaSize(SizeInt(100, 100), SizeInt(1000, 1000), SizeInt(1000, 1000));
		CHECK_CLOSE(0.1f, r.ZoomSurface, 0.0001f);
		CHECK_CLOSE(0.1f, r.ZoomImage, 0.0001f);
		CHECK_EQUAL(SizeInt(100, 100), r.ZoomedSize);
	}

	TEST_FIXTURE(SimpleReduceFixture, NoSurface) {
		r = z.CalculateViewAreaSize(SizeInt(100, 100), SizeInt(0, 0), SizeInt(1000, 1000));
		CHECK_EQUAL(0.0, r.ZoomSurface);
		CHECK_CLOSE(0.1f, r.ZoomImage, 0.0001f);
		CHECK_EQUAL(SizeInt(0, 0), r.ZoomedSize);
	}


	TEST_FIXTURE(SimpleReduceFixture, NoImage) {
		r = z.CalculateViewAreaSize(SizeInt(100, 100), SizeInt(100, 100), SizeInt(0, 0));
		CHECK_EQUAL(0.0f, r.ZoomSurface);
		CHECK_EQUAL(0.0f, r.ZoomImage);
		CHECK_EQUAL(SizeInt(0, 0), r.ZoomedSize);
	}


	TEST_FIXTURE(SimpleReduceFixture, ReduceOnlySmallSurface) {
		r = z.CalculateViewAreaSize(SizeInt(100, 100), SizeInt(10, 10), SizeInt(1000, 1000));
		CHECK_CLOSE(10.0f, r.ZoomSurface, 0.0001f);
		CHECK_CLOSE(0.1f, r.ZoomImage, 0.0001f);
	}
}