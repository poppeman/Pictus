#include "tiff_cieconverter.h"

namespace Img {
	TIFFDisplay display_sRGB = {
		{			/* XYZ -> luminance matrix */
			{  3.2410F, -1.5374F, -0.4986F },
			{  -0.9692F, 1.8760F, 0.0416F },
			{  0.0556F, -0.2040F, 1.0570F }
		},	
		100.0F, 100.0F, 100.0F,	/* Light o/p for reference white */
		255, 255, 255,		/* Pixel values for ref. white */
		1.0F, 1.0F, 1.0F,	/* Residual light o/p for black pixel */
		2.4F, 2.4F, 2.4F,	/* Gamma values for the three guns */
	};

	void TiffCieConverter::PerformCreate() {
		m_cieToRgb = (TIFFCIELabToRGB*)_TIFFmalloc(sizeof(TIFFYCbCrToRGB));

		float   *whitePoint;
		TIFFGetFieldDefaulted(m_tiff, TIFFTAG_WHITEPOINT, &whitePoint);

		float   refWhite[3];

		refWhite[1] = 100.0F;
		refWhite[0] = whitePoint[0] / whitePoint[1] * refWhite[1];
		refWhite[2] = (1.0F - whitePoint[0] - whitePoint[1]) / whitePoint[1] * refWhite[1];
		if (TIFFCIELabToRGBInit(m_cieToRgb, &display_sRGB, refWhite) < 0) {
			DO_THROW(Err::CodecError, TX("Couldn't init CIELab color conversion."));
		}
	}

	uint8_t* TiffCieConverter::PerformRequestDestinationBuffer(size_t bytes) {
		m_buffer.resize(bytes);
		return &m_buffer[0];
	}

	uint8_t* TiffCieConverter::PerformReleaseDestinationBuffer(int /*widthInPixels*/, size_t bytesUsed, size_t* expandedSize) {
		uint16_t compression = 0;
		TIFFGetField(m_tiff, TIFFTAG_COMPRESSION, &compression);

		*expandedSize = (bytesUsed * 4) / 3;
		ByteArray bufferRGB(*expandedSize);

		uint8_t* src = &m_buffer[0];
		uint32_t* dst = reinterpret_cast<uint32_t*>(&bufferRGB[0]);

		for (size_t pixel = 0; pixel < bytesUsed / 3; ++pixel) {
			uint32 cL = *(src++);
			int32 ca = *(src++);
			int32 cb = *(src++);
			float x, y, z;
			TIFFCIELabToXYZ(m_cieToRgb, cL, ca, cb, &x, &y, &z);

			uint32 r, g, b;
			TIFFXYZToRGB(m_cieToRgb, x, y, z, &r, &g, &b);

			*(dst++) = Img::ToARGBDWORD(0xff, r, g, b);
		}
		m_buffer = bufferRGB;

		return &m_buffer[0];
	}
}
