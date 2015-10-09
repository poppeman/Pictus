#include "streamconv.h"
#include "exception.h"
#include "types.h"

namespace Util {
	void StreamConverter::AddByte(const uint8_t b) {
		m_source.push_back(b);
	}

	bool StreamConverter::AddBytes(const uint8_t* pBuf, size_t bufSize) {
		if (!pBuf) {
			return false;
		}

		for (auto i = 0; i < bufSize; i++) {
			m_source.push_back(pBuf[i]);
		}

		return true;
	}

	int StreamConverter::GetWord() {
		unsigned int currchar = 0;
		unsigned int dstPos = 0;
		unsigned int copied = 0;

		do {
			if (m_source.empty()) {
				DO_THROW(Err::CriticalError, TX("Out of data!"));
			}

			uint8_t toCopy = Util::Min((uint8_t)(8 - m_srcPos), static_cast<uint8_t>(m_wordSize - copied));
			uint16_t mask = static_cast<uint16_t>((1 << toCopy) - 1) << m_srcPos;
			uint8_t currPart = static_cast<uint8_t>((m_source.front() & mask) >> m_srcPos);

			currchar |=  (currPart << dstPos);

			m_srcPos += toCopy;
			if (m_srcPos > 7) {
				m_srcPos = 0;
				m_source.pop_front();
			}

			dstPos += toCopy;
			copied += toCopy;
		}
		while (copied < m_wordSize);

		return static_cast<short>(currchar);
	}

	int StreamConverter::GetWordMsbToLsb() {
		unsigned int currchar = 0;
		unsigned int dstPos = 0;
		unsigned int copied = 0;

		do {
			if(m_source.empty()) throw Err::CriticalError(L"Out of data!");

			uint8_t toCopy = Util::Min((uint8_t)(8 - m_srcPos), static_cast<uint8_t>(m_wordSize - copied));
			uint16_t mask = static_cast<uint16_t>(((1 << toCopy) - 1) << (8 - (toCopy + m_srcPos)));
			uint8_t currPart = static_cast<uint8_t>((m_source.front() & mask) >> (8 - (toCopy + m_srcPos)));

			currchar |=  (currPart << (m_wordSize - (toCopy + dstPos)));

			m_srcPos += toCopy;
			if (m_srcPos > 7) {
				m_srcPos = 0;
				m_source.pop_front();
			}

			dstPos += toCopy;
			copied += toCopy;
		}
		while (copied < m_wordSize);

		return static_cast<short>(currchar);
	}

	bool StreamConverter::GetWordsAsByte(uint8_t* pDest, size_t num) {
		if (!pDest) {
			return false;
		}

		for (size_t i = 0; i < num; i++) {
			pDest[i] = static_cast<uint8_t>(GetWord());
		}

		return true;
	}

	bool StreamConverter::GetWordsAsByteMsbToLsb(uint8_t* pDest, size_t num) {
		if (!pDest) {
			return false;
		}

		for (size_t i = 0; i < num; i++) {
			pDest[i] = static_cast<uint8_t>(GetWordMsbToLsb());
		}

		return true;
	}

	unsigned int StreamConverter::CurrentWordSize() const {
		return m_wordSize;
	}

	void StreamConverter::ChangeWordSize(unsigned int newBitCount) {
		m_wordSize = newBitCount;
	}

	bool StreamConverter::IsWordsLeft() const {
		return (static_cast<unsigned int>(m_source.size() * 8 - m_srcPos) >= m_wordSize);
	}

	StreamConverter::StreamConverter(uint8_t destBitCount) :m_srcPos(0), m_wordSize(destBitCount) {}
}
