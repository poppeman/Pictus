#include "bitconvert.h"
#include "exception.h"
#include "streamconv.h"
#include "types.h"

namespace Util {
	void BitsToBytes(uint8_t* dest, const uint8_t* source, uint32_t numBits) {
		int wholeBytes	= numBits >> 3;
		int extraBits	= numBits & 7;

		for(int curr = 0; curr < wholeBytes; ++curr) {
			uint8_t b = *(source++);

			for(int i = 0; i < 8; i++) {
				*(dest++) = (b & 0x80) >> 7;
				b <<= 1;
			}
		}

		// Transfer any remaining bits
		uint8_t b = *source;
		for(int i = 0; i < extraBits; i++) {
			*(dest++) = (b &0x80) >> 7;
			b <<= 1;
		}
	}

	void CrumbsToBytes(uint8_t* dest, const uint8_t* source, uint32_t numCrumbs) {
		int wholeBytes	= numCrumbs >> 2;
		int extraCrumbs	= numCrumbs & 3;

		for(int curr = 0; curr < wholeBytes; ++curr) {
			uint8_t b = *(source++);
			*(dest++) = (b &0xC0) >> 6;
			*(dest++) = (b &0x30) >> 4;
			*(dest++) = (b &0x0C) >> 2;
			*(dest++) = (b &0x03);
		}

		// Transfer any remaining crumbs
		uint8_t b = *source;

		for(int i = 0; i < extraCrumbs; i++) {
			*(dest++) = (b &0xC0) >> 6;
			b<<=2;
		}
	}

	void NibblesToBytes(uint8_t* dest, const uint8_t* source, uint32_t numNibbles) {
		int wholeBytes	= numNibbles >> 1;
		int extraNibble	= numNibbles & 1;

		for(int curr = 0; curr < wholeBytes; ++curr) {
			uint8_t b = *(source++);
			*(dest++) = (b & 0xf0) >> 4;
			*(dest++) = b & 0x0f;
		}
		// Transfer any remaining bits
		if (extraNibble != 0) {
			*(dest++) = (*source & 0xf0) >> 4;
		}
	}

	void ConvertToBytes(uint8_t* dest, const uint8_t* source, uint32_t sourceBitDepth, uint32_t numSourceWords) {
		switch(sourceBitDepth) {
		case 1:
			Util::BitsToBytes(dest, source, numSourceWords);
			break;
		case 2:
			Util::CrumbsToBytes(dest, source, numSourceWords);
			break;
		case 4:
			Util::NibblesToBytes(dest, source, numSourceWords);
			break;
		default:
			if (sourceBitDepth >= 1 && sourceBitDepth <= 8) {
				// Fail safe but somewhat slow and crops the words so can't be reliably used for >8 bits.
				StreamConverter sc;
				sc.ChangeWordSize(sourceBitDepth);
				if (sc.AddBytes(source, (numSourceWords * sourceBitDepth + 7) / 8) == false) {
					DO_THROW(Err::CriticalError, "Couldn't insert source data into conversion stream.");
				}

				if (sc.GetWordsAsByteMsbToLsb(dest, numSourceWords) == false) {
					DO_THROW(Err::CriticalError, "Couldn't convert data.");
				}
			}
			else {
				DO_THROW(Err::Unsupported, "Unsupported word size: " + ToAString(sourceBitDepth));
			}
		}
	}
}
