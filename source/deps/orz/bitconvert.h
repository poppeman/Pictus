#ifndef ORZ_BITCONVERT_H
#define ORZ_BITCONVERT_H

#include "streamconv.h"

#include <cstdint>

namespace Util {
	void BitsToBytes(uint8_t* dest, const uint8_t* source, uint32_t numBits);
	void CrumbsToBytes(uint8_t* dest, const uint8_t* source, uint32_t numCrumbs);
	void NibblesToBytes(uint8_t* dest, const uint8_t* source, uint32_t numNibbles);
	void ConvertToBytes(uint8_t* dest, const uint8_t* source, uint32_t sourceBitDepth, uint32_t numSourceWords);
}

#endif
