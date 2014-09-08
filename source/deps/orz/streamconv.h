#ifndef STREAMCONV_H
#define STREAMCONV_H

namespace Util {
	class StreamConverter {
	public:
		void AddByte(_In_ const uint8_t b);
		bool AddBytes(_In_reads_bytes_(bufSize) const uint8_t* pBuf, _In_ size_t bufSize);

		_Check_return_ int GetWord();

		_Check_return_ int GetWordMsbToLsb();

		bool GetWordsAsByte(_Out_writes_all_(num) uint8_t* pDest, _In_ size_t num);

		bool GetWordsAsByteMsbToLsb(_Out_writes_all_(num) uint8_t* pDest, _In_ size_t num);

		_Check_return_ unsigned int CurrentWordSize() const;

		void ChangeWordSize(unsigned int newBitCount);

		bool IsWordsLeft() const;

		StreamConverter(uint8_t destBitCount = 8);

	private:
		typedef std::deque<uint8_t> ByteStream;
		ByteStream m_source;
		unsigned int m_wordSize;
		size_t m_srcPos;
	};
}

#endif
