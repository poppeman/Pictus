#ifndef BITCONVERT_H
#define BITCONVERT_H

#include "streamconv.h"

namespace Util {
	void BitsToBytes(_Out_writes_bytes_all_(numBits) uint8_t* dest, _In_reads_bytes_((numBits + 7) / 8) const uint8_t* source, _In_ uint32_t numBits);
	void CrumbsToBytes(_Out_writes_bytes_all_(numCrumbs) uint8_t* dest, _In_reads_bytes_((numCrumbs + 3) / 4) const uint8_t* source, _In_ uint32_t numCrumbs);
	void NibblesToBytes(_Out_writes_bytes_all_(numNibbles) uint8_t* dest, _In_reads_bytes_((numNibbles + 1) / 2) const uint8_t* source, _In_ uint32_t numNibbles);
	void ConvertToBytes(_Out_writes_bytes_all_(numSourceWords) uint8_t* dest, _In_reads_bytes_((sourceBitDepth * numSourceWords + 7) / 8) const uint8_t* source, _In_ uint32_t sourceBitDepth, uint32_t numSourceWords);
}

#endif
