#ifndef ORZ_STREAMCONV_H
#define ORZ_STREAMCONV_H

#include <cstdint>
#include <cstring>
#include <deque>

namespace Util {
	class StreamConverter {
	public:
		void AddByte(const uint8_t b);
		bool AddBytes(const uint8_t* pBuf, size_t bufSize);

		int GetWord();

		int GetWordMsbToLsb();

		bool GetWordsAsByte(uint8_t* pDest, size_t num);

		bool GetWordsAsByteMsbToLsb(uint8_t* pDest, size_t num);

		unsigned int CurrentWordSize() const;

		void ChangeWordSize(unsigned int newBitCount);

		bool IsWordsLeft() const;

		explicit StreamConverter(uint8_t destBitCount = 8);

	private:
		typedef std::deque<uint8_t> ByteStream;
		ByteStream m_source;
		unsigned int m_wordSize = 0;
		size_t m_srcPos = 0;
	};
}

#endif
