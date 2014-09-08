#include "StdAfx.h"
#include "tiff_converter.h"

namespace Img {
	void TiffDataConverter::Create(TIFF* tiff, Format dataFormat) {
		m_tiff = tiff;
		m_format = dataFormat;
		PerformCreate();
	}

	uint8_t* TiffDataConverter::RequestDestinationBuffer( size_t bytes ) {
		uint8_t* buffer = PerformRequestDestinationBuffer(bytes);
		memset(buffer, 0, bytes);
		return buffer;
	}

	uint8_t* TiffDataConverter::ReleaseDestinationBuffer( int widthInPixels, size_t bytesUsed, size_t* expandedSize ) {
		return PerformReleaseDestinationBuffer(widthInPixels, bytesUsed, expandedSize);
	}

	void TiffDataConverter::PerformCreate() {}

	TiffDataConverter::~TiffDataConverter() {}
}
