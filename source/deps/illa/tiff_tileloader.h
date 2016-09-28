#ifndef TIFF_TILELOADER_H
#define TIFF_TILELOADER_H

#include "tiff_loader.h"

namespace Img {
	class TiffTileLoader:public TiffLoader {
	private:
		TiffChunkStatus PerformLoadChunk() override;
		void PerformOnCreate() override;

		void transferTileToBuffer(uint8_t* tileData, size_t bytes);

		ttile_t m_currentTile = 0;
		ttile_t m_numberOfTiles = 0;

		int m_positionCurrentX = 0;
		size_t m_positionCurrentY = 0;
	};
}

#endif
