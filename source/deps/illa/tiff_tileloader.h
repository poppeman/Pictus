#ifndef TIFF_TILELOADER_H
#define TIFF_TILELOADER_H

#include "tiff_loader.h"

namespace Img {
	class TiffTileLoader:public TiffLoader {
	public:
		TiffTileLoader();

	private:
		TiffChunkStatus PerformLoadChunk() override;
		void PerformOnCreate() override;

		void transferTileToBuffer(uint8_t* tileData, size_t bytes);

		ttile_t m_currentTile;
		ttile_t m_numberOfTiles;

		int m_positionCurrentX;
		size_t m_positionCurrentY;
	};
}

#endif
