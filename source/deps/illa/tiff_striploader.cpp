#include "StdAfx.h"
#include "tiff_striploader.h"
#include "codec.h"
#include "surface_locked_area.h"

namespace Img {
	using namespace Geom;

	TiffStripLoader::TiffStripLoader():
		m_currentStrip{ 0 },
		m_numerOfStrips{ 0 },
		m_readScanlines{ 0 },
		m_bufferPosition{ 0 }
	{}

	TiffChunkStatus TiffStripLoader::PerformLoadChunk() {
		tsize_t currentStripSize = TIFFStripSize(m_tiff);
		if (currentStripSize == -1) {
			Log << L"(TiffStripLoader::PerformLoadChunk) Invalid strip size.";
			return TiffChunkStatus::Error;
		}

		tsize_t scanlineSize = TIFFScanlineSize(m_tiff);

		tsize_t bytesToRead = currentStripSize;

		uint8_t* destination = m_converter->RequestDestinationBuffer(bytesToRead);

		tsize_t bytesRead = TIFFReadEncodedStrip(
			m_tiff,
			m_currentStrip++,
			destination,
			bytesToRead);

		if (bytesRead == -1) {
			bytesRead = bytesToRead;
		}

		size_t expandedSize;
		uint8_t* expandedBuffer = m_converter->ReleaseDestinationBuffer(m_surface->GetSize().Width, bytesRead, &expandedSize);

		transferStripToBuffer(expandedBuffer, expandedSize);

		m_readScanlines += bytesToRead / scanlineSize;

		if (m_currentStrip == m_numerOfStrips) {
			return TiffChunkStatus::Finished;
		}

		return TiffChunkStatus::ChunksRemaining;
	}

	void TiffStripLoader::PerformOnCreate() {
		m_numerOfStrips = TIFFNumberOfStrips(m_tiff);
	}

	void TiffStripLoader::transferStripToBuffer(uint8_t* stripData, size_t bytes) {
		Surface::LockedArea::Ptr area = m_surface->LockSurface();
		uint8_t* destinationBuffer = area->Buffer();

		bytes = Util::Min(bytes, Area(m_surface->GetSize()) * m_surface->PixelSize() - m_bufferPosition);

		// Copy by scanline
		size_t destinationPitch = area->Stride();
		size_t destinationScanLength = m_surface->GetSize().Width * m_surface->PixelSize();
		size_t numScans = bytes / destinationPitch;

		for (size_t currentScanline = 0; currentScanline < numScans; ++currentScanline) {
			memcpy(destinationBuffer + m_bufferPosition, stripData, destinationScanLength);
			m_bufferPosition += destinationPitch;
			stripData += destinationScanLength;
		}

		area->Unlock();
	}
}
