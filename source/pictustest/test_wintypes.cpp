#include <UnitTest++/UnitTest++.h>
#include "Pictus/wintypes.h"

SUITE(SuiteWinTypes) {
	using namespace Win;

	TEST(KeyEventLessThanOperator) {
		KeyEvent a(WXK_SPACE, false, false, false);
		KeyEvent b(WXK_SPACE, false, true, false);
		CHECK(a < b);
		CHECK(!(b < a));
	}
}
