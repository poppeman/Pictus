#include "StdAfx.h"
#include "Pictus/wintypes.h"

SUITE(SuiteWinTypes) {
	using namespace Win;

	TEST(KeyEventLessThanOperator) {
		KeyEvent a(VK_SPACE, false, false, false);
		KeyEvent b(VK_SPACE, false, true, false);
		CHECK(a < b);
		CHECK(!(b < a));
	}
}
