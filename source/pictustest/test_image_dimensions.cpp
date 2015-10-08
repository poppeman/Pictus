#include <UnitTest++/UnitTest++.h>
#include "Pictus/image_dimensions.h"

SUITE(ImgDimSuite) {
	using App::ImageDimensions;
	using namespace Geom;
	using namespace Filter;

	struct IdFixture {
		IdFixture():imgDims(256, 512), surfDims(128, 256) {
			id.SetImageSize(imgDims);
			id.SetSurfaceSize(surfDims);
		}

		SizeInt imgDims;
		SizeInt surfDims;
		ImageDimensions id;
	};

	TEST(AngleProperty) {
		ImageDimensions id;
		CHECK_EQUAL(Filter::RotationAngle::RotateDefault, id.Angle());
		id.Angle(Filter::RotationAngle::Rotate270);
		CHECK_EQUAL(Filter::RotationAngle::Rotate270, id.Angle());
	}

	TEST_FIXTURE(IdFixture, InvalidAngle) {
		id.Angle(Filter::RotationAngle::RotateUndefined);
		CHECK_THROW(id.TranslatedSurfaceSourceRect(RectInt(PointInt(0, 0), imgDims)), Err::InvalidCall);
	}

	TEST_FIXTURE(IdFixture, SimpleImageQuery) {
		PointInt tl(32, 64);
		CHECK_EQUAL(imgDims, id.ImageSize());
		CHECK_EQUAL(surfDims, id.SurfaceSize());
		CHECK_EQUAL(imgDims, id.EffectiveSize());
		CHECK_EQUAL(RectInt(tl, imgDims), id.TranslatedSurfaceSourceRect(RectInt(tl, imgDims)));
	}

	TEST_FIXTURE(IdFixture, FullRotationShapeKeep) {
		RectInt rect(PointInt(0, 0), imgDims);
		id.Angle(RotationAngle::RotateDefault);
		CHECK_EQUAL(rect, id.TranslatedSurfaceSourceRect(rect));

		id.Angle(RotationAngle::Rotate180);
		CHECK_EQUAL(rect, id.TranslatedSurfaceSourceRect(rect));

		id.Angle(RotationAngle::FlipX);
		CHECK_EQUAL(rect, id.TranslatedSurfaceSourceRect(rect));

		id.Angle(RotationAngle::FlipY);
		CHECK_EQUAL(rect, id.TranslatedSurfaceSourceRect(rect));
	}

	TEST_FIXTURE(IdFixture, FullRotationChangeShape) {
		RectInt rect(PointInt(0, 0), imgDims.Flipped());
		RectInt rotRect(PointInt(0, 0), imgDims);

		id.Angle(RotationAngle::Rotate90);
		CHECK_EQUAL(rotRect, id.TranslatedSurfaceSourceRect(rect));

		id.Angle(RotationAngle::Rotate270);
		CHECK_EQUAL(rotRect, id.TranslatedSurfaceSourceRect(rect));
	}

/*	TEST_FIXTURE(IdFixture, Flips) {
		RectInt destRect(PointInt(10, 10), SizeInt(20, 20));
		RectInt expectedSourceRectX(PointInt(98, 10), destRect.Dimensions());
		RectInt expectedSourceRectY(PointInt(10, 226), destRect.Dimensions());
		id.Angle(Filter::FlipX);
		CHECK_EQUAL(
			expectedSourceRectX,
			id.TranslatedSurfaceSourceRect(destRect));

		id.Angle(Filter::FlipY);
		CHECK_EQUAL(
			expectedSourceRectY,
			id.TranslatedSurfaceSourceRect(destRect));
	}

	TEST_FIXTURE(IdFixture, SimpleRotations) {
		RectInt destRect(PointInt(10, 20), SizeInt(20, 30));
		RectInt expectedSourceRect90(PointInt(20, 226), SizeInt(30, 20));
		RectInt expectedSourceRect180(PointInt(98, 206), SizeInt(20, 30));
		RectInt expectedSourceRect270(PointInt(78, 10), SizeInt(30, 20));
		id.Angle(Filter::Rotate90);
		CHECK_EQUAL(
			expectedSourceRect90,
			id.TranslatedSurfaceSourceRect(destRect));

		id.Angle(Filter::Rotate180);
		CHECK_EQUAL(
			expectedSourceRect180,
			id.TranslatedSurfaceSourceRect(destRect));

		id.Angle(Filter::Rotate270);
		CHECK_EQUAL(
			expectedSourceRect270,
			id.TranslatedSurfaceSourceRect(destRect));
	}*/

	TEST_FIXTURE(IdFixture, KeepsShape) {
		RectInt destRect(PointInt(15, 12), SizeInt(33, 54));
		id.Angle(RotationAngle::Rotate180);
		CHECK_EQUAL(destRect, id.TranslatedSurfaceSourceRect(id.TranslatedSurfaceSourceRect(destRect)));
		id.Angle(RotationAngle::FlipX);
		CHECK_EQUAL(destRect, id.TranslatedSurfaceSourceRect(id.TranslatedSurfaceSourceRect(destRect)));
		id.Angle(RotationAngle::FlipY);
		CHECK_EQUAL(destRect, id.TranslatedSurfaceSourceRect(id.TranslatedSurfaceSourceRect(destRect)));
	}

	TEST_FIXTURE(IdFixture, KeepsShape90) {
		RectInt destRect(PointInt(15, 12), SizeInt(33, 54));
		id.Angle(RotationAngle::Rotate90);
		RectInt srcRect(destRect);
		for (auto i = 0; i < 4; ++i) {
			srcRect = id.TranslatedSurfaceSourceRect(srcRect);
		}
		CHECK_EQUAL(destRect, srcRect);
	}

	TEST_FIXTURE(IdFixture, KeepsShape270) {
		RectInt destRect(PointInt(15, 12), SizeInt(33, 54));
		id.Angle(RotationAngle::Rotate270);
		RectInt srcRect(destRect);
		for (auto i = 0; i < 4; ++i) {
			srcRect = id.TranslatedSurfaceSourceRect(srcRect);
		}
		CHECK_EQUAL(destRect, srcRect);
	}
}
