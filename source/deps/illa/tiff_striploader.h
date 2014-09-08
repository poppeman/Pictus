#ifndef TIFF_STRIPLOADER_H
#define TIFF_STRIPLOADER_H

#include "tiff_loader.h"

namespace Img {
	class TiffStripLoader:public TiffLoader {
	public:
		TiffStripLoader();

	private:
		TiffChunkStatus PerformLoadChunk() override;
		void PerformOnCreate() override;

		void transferStripToBuffer(uint8_t* stripData, size_t bytes);

		tstrip_t m_numerOfStrips;
		tstrip_t m_currentStrip;

		size_t m_bufferPosition;
		size_t m_readScanlines;
	};
}

#endif
