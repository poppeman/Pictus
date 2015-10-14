#include "tiff_ycbcrconverter.h"
#include "orz/types.h"
#include "libtiff/tiffiop.h"

namespace Img {
	void TiffYCbCrConverter::PerformCreate() {
		m_ycbcrToRgb = (TIFFYCbCrToRGB*)_TIFFmalloc(TIFFroundup_64(sizeof(TIFFYCbCrToRGB), sizeof(long))
			+ 4*256*sizeof(TIFFRGBValue)
			+ 2*256*sizeof(int)
			+ 3*256*sizeof(int32));

		float* luma, *refBlackWhite;
		TIFFGetFieldDefaulted(m_tiff, TIFFTAG_YCBCRCOEFFICIENTS, &luma);
		TIFFGetFieldDefaulted(m_tiff, TIFFTAG_REFERENCEBLACKWHITE, &refBlackWhite);
		TIFFYCbCrToRGBInit(m_ycbcrToRgb, luma, refBlackWhite);
	}

	uint8_t* TiffYCbCrConverter::PerformRequestDestinationBuffer(size_t bytes) {
		m_buffer = std::make_unique<uint8_t[]>(bytes);
		return &m_buffer[0];
	}

	uint8_t* TiffYCbCrConverter::PerformReleaseDestinationBuffer(int widthInPixels, size_t bytesUsed, size_t* expandedSize) {
		uint16_t subsamplingVert, subsamplingHoriz;
		TIFFGetField(m_tiff, TIFFTAG_YCBCRSUBSAMPLING, &subsamplingHoriz, &subsamplingVert);

		// FIXME: Do not assume striped data
		if (subsamplingHoriz == 2 && subsamplingVert == 1) {
			uint32_t h = bytesUsed / (widthInPixels * 2);

			*expandedSize = bytesUsed * 2;
			ByteArray bufferRGB;
			bufferRGB = std::make_unique<uint8_t[]>(*expandedSize);

			uint8_t* src = &m_buffer[0];
			uint32_t* dst = reinterpret_cast<uint32_t*>(&bufferRGB[0]);

			for (size_t y = 0; y < h; ++y) {
				for (int x = 0; x < (widthInPixels / 2); ++x) {
					int32 Cb = src[2];
					int32 Cr = src[3];

					uint32 R, G, B;
					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[0], Cb, Cr, &R, &G, &B);
					*(dst++) = Img::ToARGBDWORD(0xff, R, G, B);

					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[1], Cb, Cr, &R, &G, &B);
					*(dst++) = Img::ToARGBDWORD(0xff, R, G, B);

					src += 4;
				}
			}
			m_buffer.swap(bufferRGB);
		}
		else if (subsamplingHoriz == 2 && subsamplingVert == 2) {
			ptrdiff_t h = (bytesUsed * 4) / (widthInPixels * 6);

			*expandedSize = (bytesUsed * 16) / 6;
			auto bufferRGB(std::make_unique<uint8_t[]>(*expandedSize));

			uint8_t* src = &m_buffer[0];
			uint32_t* dstCurr = reinterpret_cast<uint32_t*>(&bufferRGB[0]);
			uint32_t* dstNext = dstCurr + widthInPixels;

			for (int y = 0; y < (h / 2); ++y) {
				for (int x = 0; x < (widthInPixels / 2); ++x) {
					int32 Cb = src[4];
					int32 Cr = src[5];

					uint32 R, G, B;
					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[0], Cb, Cr, &R, &G, &B);
					*(dstCurr++) = Img::ToARGBDWORD(0xff, R, G, B);

					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[1], Cb, Cr, &R, &G, &B);
					*(dstCurr++) = Img::ToARGBDWORD(0xff, R, G, B);

					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[2], Cb, Cr, &R, &G, &B);
					*(dstNext++) = Img::ToARGBDWORD(0xff, R, G, B);

					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[3], Cb, Cr, &R, &G, &B);
					*(dstNext++) = Img::ToARGBDWORD(0xff, R, G, B);

					src += 6;
				}
				dstCurr += widthInPixels;
				dstNext += widthInPixels;
			}

			if ((h % 2) == 1) {
				for (int x = 0; x < (widthInPixels / 2); ++x) {
					int32 Cb = src[4];
					int32 Cr = src[5];
					uint32 R, G, B;
					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[0], Cb, Cr,  &R, &G, &B);
					*(dstCurr++) = Img::ToARGBDWORD(0xff, R, G, B);

					TIFFYCbCrtoRGB(m_ycbcrToRgb, src[1], Cb, Cr, &R, &G, &B);
					*(dstCurr++) = Img::ToARGBDWORD(0xff, R, G, B);
					src += 6;
				}
			}

			m_buffer.swap(bufferRGB);
		}
		else
			DO_THROW(Err::Unsupported, "Subsampling (" + ToAString(subsamplingHoriz) + ", " + ToAString(subsamplingVert) + ") not yet supported");

		return &m_buffer[0];
	}
}
