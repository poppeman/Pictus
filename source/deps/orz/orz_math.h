#ifndef ORZ_MATH_H
#define ORZ_MATH_H

#include <cstdint>

namespace Math {
	class Randomizer {
	public:
		void Seed(uint32_t seed);
		void SeedTimer();

		unsigned long Random();

		Randomizer();
		Randomizer(uint32_t seed);

	private:
		void RecalcMersenneSet();

		enum {
			NumbersPerBatch = 624,
			SomeConst		= 397,
			BitMask32		= 0xffffffffUL,
			TopBit			= 0x80000000UL,
			LowerBits		= 0x7fffffffUL
		};

		unsigned long m_mersenne[NumbersPerBatch];
		int m_currIndex;
		static const unsigned long GenSwitch[2];
	};

	namespace Bit {
		int CountSet(uint32_t u);
 	}

	int Lerp(float t, int a, int b);
}

#endif
