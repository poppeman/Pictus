#include "StdAfx.h"
#include "orz_math.h"

namespace Math {
	const unsigned long Randomizer::GenSwitch[2] =  {0, 0x9908b0dfUL};
		// Unseeded by default.

	_Use_decl_annotations_ void Randomizer::Seed(uint32_t seed) {
		m_mersenne[0] = seed & BitMask32;
		for (int i = 1; i < NumbersPerBatch; i++) {
			m_mersenne[i] = (1812433253UL * (m_mersenne[i - 1] ^ (m_mersenne[i - 1] >> 30)) + i) & BitMask32;
		}

		m_currIndex = NumbersPerBatch;
	}

	void Randomizer::SeedTimer() {
		struct _timeb tb;
		(void)_ftime_s(&tb);
		Seed(static_cast<unsigned long>(tb.time * 1000 + tb.millitm));
	}

	void Randomizer::RecalcMersenneSet() {
		// Perform initial seed.
		if (m_currIndex > NumbersPerBatch) {
			SeedTimer();
		}

		// First part
		for(int i = 0; i < NumbersPerBatch - SomeConst; i++) {
			unsigned long y = (m_mersenne[i] & TopBit) + (m_mersenne[i + 1] & LowerBits);
			m_mersenne[i] = m_mersenne[i + SomeConst] ^ (y >> 1) ^ GenSwitch[y & 0x01];
		}

		for(int i = NumbersPerBatch - SomeConst; i < NumbersPerBatch - 1; i++) {
			unsigned long y = (m_mersenne[i] & TopBit) | (m_mersenne[i + 1] & LowerBits);
			m_mersenne[i] = m_mersenne[i + (SomeConst - NumbersPerBatch)] ^ (y >> 1) ^ GenSwitch[y & 0x01];
		}

		unsigned long y = (m_mersenne[NumbersPerBatch - 1] & TopBit) | (m_mersenne[0] & LowerBits);
		m_mersenne[NumbersPerBatch - 1] = m_mersenne[SomeConst - 1] ^ (y >> 1) ^ GenSwitch[y & 0x01];

		// Reset counter.
		m_currIndex=0;
	}

	_Use_decl_annotations_ unsigned long Randomizer::Random() {
		// Recalculate if needed
		if (m_currIndex >= NumbersPerBatch) {
			RecalcMersenneSet();
			m_currIndex=0;
		}

		// Fetch a single number from the set. Recalculate if needed
		unsigned long curr = m_mersenne[m_currIndex++];
		curr = curr ^ (curr >> 11);
		curr = curr ^ ((curr << 7) & 0x9d2c5680);
		curr = curr ^ ((curr << 15) & 0xefc60000);
		curr = curr ^ (curr >> 18);
		return curr;
	}

	Randomizer::Randomizer():
		m_currIndex{ NumbersPerBatch + 1 }
	{
		for(int i = 0; i < NumbersPerBatch; ++i)
			m_mersenne[i] = 0;
	}

	Randomizer::Randomizer(uint32_t seed) {
		Seed(seed);
	}

	_Check_return_ int Lerp( _In_ float t, _In_ int a, _In_ int b )
	{
		int size = b - a;
		return static_cast<int>(a + size * t);
	}

	int Bit::CountSet(uint32_t u) {
		uint32_t uCount = u - (((u >> 1) & 033333333333) + ((u >> 2) & 011111111111));
		return ((uCount + (uCount >> 3)) & 030707070707) % 63;
	}

}
