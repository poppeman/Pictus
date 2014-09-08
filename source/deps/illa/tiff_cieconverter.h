#ifndef TIFF_CIECONVERTER_H
#define TIFF_CIECONVERTER_H

#include "tiff_converter.h"

namespace Img {
	class TiffCieConverter:public TiffDataConverter {
		void PerformCreate();
		uint8_t* PerformRequestDestinationBuffer(size_t bytes);
		uint8_t* PerformReleaseDestinationBuffer(int widthInPixels, size_t bytesUsed, size_t* expandedSize);	

		typedef std::vector<uint8_t> ByteArray;
		ByteArray					m_buffer;
		TIFFCIELabToRGB*			m_cieToRgb;
	};
}

#endif
