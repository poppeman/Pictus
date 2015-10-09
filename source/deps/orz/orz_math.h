#ifndef ORZ_MATH_H
#define ORZ_MATH_H

#include <cstdint>

namespace Math {
	namespace Bit {
		int CountSet(uint32_t u);
 	}

	int Lerp(float t, int a, int b);
}

#endif
