#include <UnitTest++/UnitTest++.h>
#include "illa/filter_bilinear.h"

using namespace Filter;
using namespace Filter::Scale::Internal;

SUITE(Bilinear) {
	TEST(CalcContribsSmallRect) {
		std::vector<Contrib> cs(2);
		set_contrib(cs, 0, 2, 1, 0);
		CHECK_EQUAL(0, cs[0].floor);
	}
}
