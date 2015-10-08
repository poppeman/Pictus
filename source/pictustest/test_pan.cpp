#include <UnitTest++/UnitTest++.h>
#include "Pictus/view_pan.h"

SUITE(Panning) {
	using App::ViewPan;
	using namespace Geom;

	TEST(DefaultState) {
		ViewPan p;
		CHECK(IsZeroOrPositive(p.TopLeft()));
	}

	TEST(SimplePan) {
		ViewPan p;
		p.ResizeConstraints(SizeInt(100, 200));
		p.Pan(SizeInt(10, 20));
		CHECK(p.TopLeft() == PointInt(10, 20));
	}

	TEST(SimpleConstrains) {
		ViewPan p;
		p.ResizeViewport(SizeInt(50, 50));
		p.ResizeConstraints(SizeInt(100, 100));
		p.Pan(SizeInt(90, 90));
		CHECK_EQUAL(PointInt(100, 100), p.BottomRight());
		CHECK_EQUAL(PointFloat(75, 75), p.Center());
		CHECK_EQUAL(PointInt(50, 50), p.TopLeft());
	}

	TEST(CenterError) {
		ViewPan p;
		p.ResizeViewport(SizeInt(481, 361));
		p.ResizeConstraints(SizeInt(481, 361));
		CHECK_EQUAL(PointFloat(240, 180), p.Center());
	}

	TEST(SimpleResize) {
		ViewPan p;
		p.ResizeConstraints(SizeInt(10000, 10000));
		p.Pan(SizeInt(800, 900));
		Geom::PointFloat prevCenter = p.Center();
		p.ResizeViewport(SizeInt(300, 600));
		CHECK_EQUAL(prevCenter, p.Center());
		CHECK_EQUAL(PointInt(650, 600), p.TopLeft());
	}


	TEST(ChangeConstraints) {
		ViewPan p;
		p.ResizeViewport(SizeInt(100, 100));
		p.ResizeConstraints(SizeInt(500, 500));
		p.SetCenter(PointInt(250, 250));
		p.ResizeConstraints(SizeInt(1000, 1000));
		CHECK_EQUAL(PointFloat(500, 500), p.Center());
	}

	struct FixtureRound {
		FixtureRound() {
			p.ResizeViewport(SizeInt(717, 448));
			p.ResizeConstraints(SizeInt(717, 448));
		}
		ViewPan p;
	};

	TEST_FIXTURE(FixtureRound, PanRoundUp) {
		p.Pan(SizeInt(1, 1));
		CHECK_EQUAL(PointInt(0, 0), p.TopLeft());
		CHECK_EQUAL(PointInt(717, 448), p.BottomRight());
	}

	TEST_FIXTURE(FixtureRound, PanRoundDown) {
		p.Pan(SizeInt(-1, -1));
		CHECK_EQUAL(PointInt(0, 0), p.TopLeft());
		CHECK_EQUAL(PointInt(717, 448), p.BottomRight());
	}
}
