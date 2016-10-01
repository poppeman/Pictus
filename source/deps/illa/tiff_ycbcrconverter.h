#ifndef TIFF_YCBCRCONVERTER_H
#define TIFF_YCBCRCONVERTER_H

#include "tiff_converter.h"

namespace Img {
	class TiffYCbCrConverter:public TiffDataConverter {
		void						PerformCreate();
		uint8_t*						PerformRequestDestinationBuffer(size_t bytes) override;
		uint8_t*						PerformReleaseDestinationBuffer(unsigned int widthInPixels, size_t bytesUsed, size_t* expandedSize) override;

		typedef std::unique_ptr<uint8_t[]> ByteArray;
		ByteArray					m_buffer;
		TIFFYCbCrToRGB*				m_ycbcrToRgb = nullptr;
	};
}

#endif
