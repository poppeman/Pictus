#include "StdAfx.h"
#include "tiff_tileloader.h"

namespace Img {
	TiffTileLoader::TiffTileLoader()
		:m_positionCurrentX(0),
		 m_positionCurrentY(0),
		 m_currentTile(0)
	{}

	TiffChunkStatus TiffTileLoader::PerformLoadChunk() {
		tsize_t currentTileSize = TIFFTileSize(m_tiff);
		uint8_t* destination = m_converter->RequestDestinationBuffer(currentTileSize);

		tsize_t bytesRead = TIFFReadEncodedTile(m_tiff, m_currentTile++, destination, currentTileSize);

		if (bytesRead == -1) {
			Log << L"(TiffTileLoader::PerformLoadChunk) Failed decoding tile";
			return TiffChunkStatus::Error;
		}

		size_t expandedSize;
		uint32_t tileWidth;
		TIFFGetField(m_tiff, TIFFTAG_TILEWIDTH, &tileWidth);
		uint8_t* expandedBuffer = m_converter->ReleaseDestinationBuffer(tileWidth, bytesRead, &expandedSize);

		transferTileToBuffer(expandedBuffer, expandedSize);

		if (m_currentTile == m_numberOfTiles) {
			return TiffChunkStatus::Finished;
		}

		return TiffChunkStatus::ChunksRemaining;
	}

	void TiffTileLoader::PerformOnCreate() {
		m_numberOfTiles = TIFFNumberOfTiles(m_tiff);
	}

	template <typename T>
	void transferPixels(size_t tileWidth, size_t tileHeight, size_t positionCurrentX, size_t positionCurrentY, size_t width, T* source, T* dest) {
		dest += positionCurrentX + positionCurrentY * width;

		for (size_t y = 0; y < tileHeight; ++y) {
			T* currentRow = dest + y * width;
			for (size_t x = 0; x < tileWidth; ++x) {
				*(currentRow++) = *(source++);
			}
		}
	}

	void TiffTileLoader::transferTileToBuffer( uint8_t* tileData, size_t tileDataSize ) {
		uint32_t maxTileHeight = 0;
		TIFFGetField(m_tiff, TIFFTAG_TILELENGTH, &maxTileHeight);

		uint32_t maxTileWidth = 0;
		TIFFGetField(m_tiff, TIFFTAG_TILEWIDTH, &maxTileWidth);

		size_t tileHeight	= Util::Min<size_t>(m_surface->GetSize().Height - m_positionCurrentY, maxTileHeight);
		size_t tileWidth	= Util::Min<size_t>(m_surface->GetSize().Width - m_positionCurrentX, maxTileWidth);

		Surface::LockedArea::Ptr area = m_surface->LockSurface();
		uint8_t* dest = area->Buffer();

		size_t surfaceWidth = m_surface->GetSize().Width;

		switch(m_surface->PixelSize()) {
			case 1:
				transferPixels<uint8_t>(tileWidth, tileHeight, m_positionCurrentX, m_positionCurrentY, surfaceWidth, tileData, dest);
				break;
			case 4:
				transferPixels<uint32_t>(tileWidth, tileHeight, m_positionCurrentX, m_positionCurrentY, surfaceWidth, reinterpret_cast<uint32_t*>(tileData), reinterpret_cast<uint32_t*>(dest));
				break;
		}

		area->Unlock();

		m_positionCurrentX += tileWidth;
		if (m_positionCurrentX == m_surface->Width()) {
			m_positionCurrentX = 0;
			m_positionCurrentY += tileHeight;
		}
	}
}
