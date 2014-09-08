#include "StdAfx.h"

#include "orz/orz_math.h"
#include "mt19937ar.h"

SUITE(Random)
{
	TEST(Random_MT)
	{
		// Compare a set to the reference implementation
		init_genrand(285813);

		Math::Randomizer rnd(285813);
		for(int i = 0; i < 624; ++i)
		{
			CHECK(genrand_int32() == rnd.Random());
		}
	}
}
