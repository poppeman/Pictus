#include "orz_math.h"

namespace Math {
	int Lerp(float t, int a, int b )
	{
		int size = b - a;
		return static_cast<int>(a + size * t);
	}

	int Bit::CountSet(uint32_t u) {
		uint32_t uCount = u - (((u >> 1) & 033333333333) + ((u >> 2) & 011111111111));
		return ((uCount + (uCount >> 3)) & 030707070707) % 63;
	}
}
