#include "orz/matrix.h"
#include <UnitTest++/UnitTest++.h>

SUITE(MAtrix)
{
	TEST(DefaultState)
	{
		Hw3D::Matrix m;
		for (auto y = 0; y < 4; y++) {
			for (auto x = 0; x < 4; x++) {
				CHECK_EQUAL(0.0f, m.m[y][x]);
			}
		}
	}

	TEST(Identity)
	{
		auto m = Hw3D::Identity();
		for (auto y = 0; y < 4; y++) {
			for (auto x = 0; x < 4; x++) {
				CHECK_EQUAL((x == y)?1.0f:0.0f, m.m[y][x]);
			}
		}
	}
}
