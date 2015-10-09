#include <UnitTest++/UnitTest++.h>
#include "illa/types.h"

using Geom::RectInt;
using Geom::SizeInt;
using Geom::PointInt;

/*******
	RECT
	*******/

SUITE(RectangleSuite)
{

	TEST(Default_Ctor)
	{
		Geom::RectInt r;
		CHECK(r.Width() == 0);
		CHECK(r.Height() == 0);
	}

	TEST(Ctor_Point_Size)
	{
		Geom::RectInt r(Geom::PointInt(300, 400), Geom::SizeInt(100, 200));
		CHECK(r.Width() == 100);
		CHECK(r.Height() == 200);
		CHECK(r.Left() == 300);
		CHECK(r.Top() == 400);
	}

	TEST(Ctor_Point_Point)
	{
		Geom::RectInt r(Geom::PointInt(300, 400), Geom::PointInt(100, 200));
		CHECK(r.Width() == 200);
		CHECK(r.Height() == 200);
		CHECK(r.Left() == 100);
		CHECK(r.Top() == 200);
	}

	TEST(Consistency)
	{
		Geom::RectInt r(PointInt(500, 100), SizeInt(100, 200));
		CHECK(r.Width() == r.Right() - r.Left());
		CHECK(r.Height() == r.Bottom() - r.Top());
	}

	TEST(Crop) {
		RectInt r1(PointInt(100, 200), PointInt(300, 400));
		RectInt r2(PointInt(0, 10), PointInt(333, 323));
		CHECK_EQUAL(RectInt(PointInt(100, 200), PointInt(300, 323)), r1.Crop(r2));
		CHECK_EQUAL(r1.Crop(r2), r2.Crop(r1));
	}

	TEST(ClipSize) {
		RectInt dstCanvas{ { 0, 0 }, PointInt{ 400, 400 } };
		RectInt srcCanvas{ { 0, 0 }, PointInt{ 200, 200 } };

		auto a = Geom::ClipSource(dstCanvas, PointInt{ 100, 200 }, srcCanvas, PointInt{ 50, 50 }, SizeInt{ 50, 50 });
		CHECK_EQUAL(RectInt({ 50, 50 }, PointInt{ 100, 100 }), a);

		auto b = Geom::ClipSource(dstCanvas, PointInt{ 100, 200 }, srcCanvas, PointInt{ 50, 50 }, SizeInt{ 999, 999 });
		CHECK_EQUAL(RectInt({ 50, 50 }, PointInt{ 200, 200 }), b);
	}
}

/*******
	PointInt
	*******/
SUITE(Point)
{
	TEST(Default_Ctor)
	{
		PointInt p;
		CHECK(p.X == 0);
		CHECK(p.Y == 0);
	}

	TEST(Flipping) {
		PointInt p(9, 4);
		PointInt q = p.Flipped();
		CHECK_EQUAL(9, p.X);
		CHECK_EQUAL(4, p.Y);
		CHECK_EQUAL(4, q.X);
		CHECK_EQUAL(9, q.Y);
	}

	TEST(Comp)
	{
		PointInt p1(5, 7), p2(5, 9), p3(6, 7), p4(7, 5), p5(5, 7);
		CHECK(p1 != p2);
		CHECK(p1 != p3);
		CHECK(p1 != p4);
		CHECK(p1 == p5);
	}

	TEST(Add)
	{
		PointInt p1(5, 7), p2(15, 23);
		PointInt p3 = p1 + p2;
		CHECK(p3 == PointInt(20, 30));

		p1 += p2;
		CHECK(p1 == p3);
	}

	TEST(Sub)
	{
		PointInt p1(5, 7), p2(15, 23);
		SizeInt p3 = p1 - p2;
		CHECK(p3 == SizeInt(-10, -16));

		p1 -= SizeInt(p1.X, p1.Y);
		CHECK(p1 == PointInt(0, 0));
	}
}




/*******
	SizeInt
	*******/
SUITE(Size)
{
	TEST(Default_Ctor)
	{
		SizeInt s;
		CHECK(s.Width == 0);
		CHECK(s.Height == 0);
	}

	TEST(Flipping) {
		SizeInt p(9, 4);
		SizeInt q = p.Flipped();
		CHECK_EQUAL(9, p.Width);
		CHECK_EQUAL(4, p.Height);
		CHECK_EQUAL(4, q.Width);
		CHECK_EQUAL(9, q.Height);
	}

	TEST(Comp)
	{
		SizeInt p1(5, 7), p2(5, 9), p3(6, 7), p4(7, 5), p5(5, 7);
		CHECK(p1 != p2);
		CHECK(p1 != p3);
		CHECK(p1 != p4);
		CHECK(p1 == p5);
	}

	TEST(Add)
	{
		SizeInt p1(5, 7), p2(15, 23);
		SizeInt p3 = p1 + p2;
		CHECK(p3 == SizeInt(20, 30));
	}

	TEST(Sub)
	{
		SizeInt p1(5, 7), p2(15, 23);
		SizeInt p3 = p1 - p2;
		CHECK(p3 == SizeInt(-10, -16));
	}

	TEST(TripleMin) {
		CHECK_EQUAL(SizeInt(-5, -6), Minimum(SizeInt(30, -6), SizeInt(-2, 40), SizeInt(-5, 20)));
	}
}
