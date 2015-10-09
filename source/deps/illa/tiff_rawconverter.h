#ifndef TIFF_RAWCONVERTER_H
#define TIFF_RAWCONVERTER_H

#include "tiff_converter.h"
#include <vector>

namespace Img {
	class TiffRawConverter:public TiffDataConverter {
		uint8_t*						PerformRequestDestinationBuffer(size_t bytes);
		uint8_t*						PerformReleaseDestinationBuffer(int widthInPixels, size_t bytesUsed, size_t* expandedSize);	

		typedef std::vector<uint8_t> ByteArray;
		ByteArray					m_buffer;
	};
}

#endif
