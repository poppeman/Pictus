#include "tiff_rawconverter.h"
#include "orz/bitconvert.h"

namespace Img {
	uint8_t* TiffRawConverter::PerformRequestDestinationBuffer(size_t bytes) {
		m_buffer.resize(bytes);
		return &m_buffer[0];
	}

	uint8_t* TiffRawConverter::PerformReleaseDestinationBuffer(int widthInPixels, size_t bytesUsed, size_t* expandedSize) {
		if (m_buffer.empty()) {
			DO_THROW(Err::CriticalError, "Buffer not created.");
		}

		*expandedSize = bytesUsed;

		uint16_t samplesPerPixel = 0;
		TIFFGetField(m_tiff, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
		uint16_t bitsPerSample = 0;
		TIFFGetField(m_tiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

		if (samplesPerPixel == 1 && bitsPerSample < 8) {
			size_t stride = (widthInPixels * bitsPerSample + 7)/ 8;
			size_t numRows = bytesUsed / stride;

			*expandedSize = widthInPixels * numRows;
			ByteArray destData(*expandedSize);

			uint8_t* sourceRow = &m_buffer[0];
			uint8_t* destRow = &destData[0];

			for (size_t currentRow = 0; currentRow < numRows; ++currentRow){
				Util::ConvertToBytes(destRow, sourceRow, bitsPerSample, widthInPixels);
				destRow		+= widthInPixels;
				sourceRow	+= stride;
			}

			m_buffer = destData;
		}
		else if (samplesPerPixel == 3 && bitsPerSample == 8) {
			*expandedSize = (bytesUsed * 4) / 3;
			ByteArray destData(*expandedSize);

			uint8_t* sourceData = reinterpret_cast<uint8_t*>(&m_buffer[0]);
			uint32_t* destinationData = reinterpret_cast<uint32_t*>(&destData[0]);

			for (unsigned pixel = 0; pixel < (bytesUsed / 3); ++pixel) {
				*(destinationData++) = Img::ToARGBDWORD(Img::Color(0xff, sourceData[0], sourceData[1], sourceData[2]));
				sourceData += 3;
			}

			m_buffer = destData;
		}
		else if (samplesPerPixel == 4 && bitsPerSample == 8) {
			uint32_t* pDScan	= reinterpret_cast<uint32_t*>(&m_buffer[0]);

			for(size_t pixel = 0; pixel < (bytesUsed / 4); pixel++) {
				uint32_t currpix = pDScan[pixel];
				pDScan[pixel] = Img::ABGR_ARGB(currpix);
			}
		}
		else if (samplesPerPixel== 1 && bitsPerSample == 16) {
			uint16_t* sourceData = reinterpret_cast<uint16_t*>(&m_buffer[0]);

			*expandedSize = bytesUsed >> 1;
			ByteArray destData(*expandedSize);

			for (unsigned currSample = 0; currSample < *expandedSize; ++currSample) {
				destData[currSample] = static_cast<uint8_t>(sourceData[currSample] >> 8);
			}

			m_buffer = destData;
		}

		return &m_buffer[0];
	}
}
