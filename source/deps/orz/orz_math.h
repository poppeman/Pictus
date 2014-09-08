#ifndef ORZ_MATH_H
#define ORZ_MATH_H

namespace Math {
	class Randomizer {
	public:
		void Seed(_In_ uint32_t seed);
		void SeedTimer();

		_Check_return_ unsigned long Random();

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
		_Check_return_ inline int CountSet(_In_ uint32_t u) {
			uint32_t uCount = u - (((u >> 1) & 033333333333) + ((u >> 2) & 011111111111));
			return ((uCount + (uCount >> 3)) & 030707070707) % 63;
		}
 	}

	_Check_return_ int Lerp(_In_ float t, _In_ int a, _In_ int b);
}

#endif
