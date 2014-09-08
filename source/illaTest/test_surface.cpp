#include "StdAfx.h"

#include "illa/surfacemgr.h"
#include "illa/swsurface.h"

SUITE(SurfaceSuite) {
	struct SurfaceTestFixture
	{
		SurfaceTestFixture()
			:s(Img::CreateNewSurface(Geom::SizeInt(10, 10), Img::Format::XRGB8888))
		{}

		Img::Surface::Ptr s;
	};

	struct SurfaceTestFixtureIndex8
	{
		SurfaceTestFixtureIndex8()
			:s(Img::CreateNewSurface(Geom::SizeInt(10, 10), Img::Format::Index8))
		{}

		Img::Surface::Ptr s;
	};

	using namespace Geom;
	using namespace Img;

	TEST(DefaultState) {
		Img::Surface::Ptr p(Img::CreateNewSurface());
		CHECK_EQUAL(0, p->PixelSize());
		CHECK_EQUAL(Img::Format::Undefined, p->GetFormat());
		CHECK_EQUAL(true, p->IsDirty());
		CHECK_EQUAL(SizeInt(0, 0), p->GetSize());
	}

	TEST_FIXTURE(SurfaceTestFixture, DoubleCreate) {
		CHECK_THROW(s->CreateSurface(Geom::SizeInt(10, 10), Img::Format::ARGB1555), Err::CriticalError);
	}

	TEST_FIXTURE(SurfaceTestFixture, DefaultDirtyState)
	{
		CHECK(s->IsDirty());
	}

	TEST_FIXTURE(SurfaceTestFixture, No32BitPalette)
	{
		s->GetPalette();
		Img::Palette p;
		s->SetPalette(p);
		CHECK(true); // Suppress warning
	}

	TEST_FIXTURE(SurfaceTestFixtureIndex8, PaletteChangesStick)
	{
		const Img::Color colRef(133, 100, 32, 85);
		const int index = 3;
		Img::Palette p = s->GetPalette();
		p.Color(index, colRef);
		s->SetPalette(p);
		CHECK_EQUAL(s->GetPalette().Color(index), colRef);
	}

	TEST_FIXTURE(SurfaceTestFixtureIndex8, PropertiesIndex8)
	{
		CHECK_EQUAL(s->PixelSize(), 1);
		CHECK_EQUAL(s->GetFormat(), Img::Format::Index8);
	}

	TEST_FIXTURE(SurfaceTestFixture, PixelSize32Bit)
	{
		CHECK_EQUAL(s->PixelSize(), 4);
		CHECK_EQUAL(s->GetFormat(), Img::Format::XRGB8888);
	}

	TEST_FIXTURE(SurfaceTestFixture, MatchingSizes)
	{
		CHECK_EQUAL(s->GetSize().Width, s->Width());
		CHECK_EQUAL(s->GetSize().Height, s->Height());
	}

	TEST(SurfaceDefaultPointer) {
		Img::SurfaceSoftware::Ptr srf(new Img::SurfaceSoftware());
		CHECK(srf->DeviceIndependentData() == 0);
	}

	TEST_FIXTURE(SurfaceTestFixture, TooLargeRect) {
		CHECK_THROW(s->LockSurface(RectInt(PointInt(0, 0), SizeInt(11, 11))), Err::InvalidParam);
	}

	TEST_FIXTURE(SurfaceTestFixture, OutOfBoundsLockRect) {
		CHECK_THROW(s->LockSurface(RectInt(PointInt(20, 30), SizeInt(5, 5))), Err::InvalidParam);
	}
}
