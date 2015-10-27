#include "c_psd.h"
#include "orz/bitconvert.h"
#include "orz/logger.h"
#include "psd_shared.h"
#include "surface_locked_area.h"

#include <cstring>

namespace Img {
	using namespace Geom;
	using namespace Internal;

	bool CodecPSD::PerformLoadHeader( IO::FileReader::Ptr file, ImageInfo& info ) {
		m_currentScanLine = 0;
		m_currentChannel = 0;
		m_currentRleStreamIndex = 0;
		m_palette = Img::Grayscale();

		if (m_header.ReadHeader(file) == false)
		{
			return false;
		}

		if (m_header.Width == 0 || m_header.Height == 0)
		{
			return false;
		}

		if (m_header.BitsPerChannel > 8)
		{
			Log << "(CodecPSD::PerformLoadHeader) BitsPerChannel not supported, was " << m_header.BitsPerChannel << "\n";
			return false;
		}

		info.Dimensions = Geom::SizeInt(m_header.Width, m_header.Height);
		info.SurfaceFormat = determineSurfaceFormat();

		// Hard-wired for RGB images
		uint32_t colorDataLength = ReadDWordBE(file);

		unsigned int numEntries = colorDataLength / 3;

		if (numEntries > 256) {
			return false;
		}

		uint8_t rawColorData[768];
		if (file->Read(rawColorData, 3, numEntries) != numEntries) {
			return false;
		}

		for (unsigned i = 0; i < numEntries; ++i)
			m_palette.Color(i, Img::Color(0xff, rawColorData[i], rawColorData[256 + i], rawColorData[512 + i]));

		uint32_t resourcesDataLength = ReadDWordBE(file);
		file->Seek(resourcesDataLength, IO::SeekMethod::Current);

		uint32_t miscInfoDataLength = ReadDWordBE(file);
		file->Seek(miscInfoDataLength, IO::SeekMethod::Current);

		m_compression = PSCompressionMode(ReadWordBE(file));

		if (m_compression != CompressionRle && m_compression != CompressionRaw) {
			return false;
		}
		m_colorPlaneSize = (m_header.BitsPerChannel >> 3) * Geom::Area(info.Dimensions);

		if (m_compression == CompressionRle) {
			if (m_header.Channels == 0)
			{
				return false;
			}
			m_numRleEntries = m_header.Height * m_header.Channels;
			m_rleScanlineLength.resize(m_numRleEntries);
			file->ReadFull(&m_rleScanlineLength.front(), 2 * m_numRleEntries);

			size_t maxLength = 0;

			for (int i = 0; i < m_numRleEntries; ++i) {
				uint8_t* data = reinterpret_cast<uint8_t*>(&m_rleScanlineLength[i]);

				m_rleScanlineLength[i] = (data[0] << 8) + data[1];
				if (m_rleScanlineLength[i] > maxLength) maxLength = m_rleScanlineLength[i];
			}
			m_currentRleStreamData.resize(maxLength);
		}

		if (m_header.ColorMode == PsdHeader::ColorModeBitmap) {
			m_palette.Color(0, Img::Color(0xff, 0xff, 0xff, 0xff));
			m_palette.Color(1, Img::Color(0xff, 0, 0, 0));
		}

		m_imageDataOffset = file->Position();

		return true;
	}

	AbstractCodec::AllocationStatus CodecPSD::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());
		GetSurface()->ClearSurface(Img::Color(0xff, 0xff, 0xff, 0xff));
		if (GetFormat() == Format::Index8) {
			GetSurface()->SetPalette(m_palette);
		}

		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecPSD::PerformLoadImageData(IO::FileReader::Ptr file) {
		switch(m_header.ColorMode) {
			case PsdHeader::ColorModeRgb:
			case PsdHeader::ColorModeIndexed:
			case PsdHeader::ColorModeGrayscale:
			case PsdHeader::ColorModeBitmap:
				return loadRgb(file);
		}
		return LoadStatus::Failed;
	}

	AbstractCodec::LoadStatus CodecPSD::loadRgb( IO::FileReader::Ptr file ) {
		auto currPlaneData = std::make_unique<uint8_t[]>(GetSize().Width);

		while(DoTerminate() == false) {
			int scansToProcess = std::min<int>(GetSurface()->Height() - m_currentScanLine, ScansPerChunk);
			std::shared_ptr<Surface::LockedArea> area = GetSurface()->LockSurface(RectInt(PointInt(0, m_currentScanLine), SizeInt(GetSurface()->Width(), scansToProcess)));
			uint8_t* currChunkStart = area->Buffer();

			size_t bytesPerScanline = (m_header.BitsPerChannel * m_header.Width + 7) >> 3;

			uint8_t* currScanStart = currChunkStart;
			if (m_header.ColorMode == PsdHeader::ColorModeRgb) {
				currScanStart += 2 - m_currentChannel;

				// Compensate for non-matching order of channels.
				if (m_header.Channels >= 4 && (m_currentChannel == 3))
					currScanStart += 4;
			}

			for (int currScan = 0; currScan < scansToProcess; ++currScan) {
				switch(m_compression) {
					case CompressionRle:
						loadRleStream(file, currPlaneData.get(), bytesPerScanline);
						break;
					case CompressionRaw:
						loadRawStream(file, currPlaneData.get(), bytesPerScanline);
						break;
				}

				switch(m_header.ColorMode) {
					case PsdHeader::ColorModeRgb:
						currScanStart = transferRgbScanlinePlane(currScanStart, currPlaneData.get(), GetSize().Width);
						break;
					case PsdHeader::ColorModeIndexed:
					case PsdHeader::ColorModeGrayscale:
						currScanStart = transferIndexedScanline(currScanStart, currPlaneData.get(), GetSize().Width);
						break;
					case PsdHeader::ColorModeBitmap:
						currScanStart = transferBitmapScanline(currScanStart, currPlaneData.get(), GetSize().Width);
						break;
				}
			}

			area->Unlock();
			m_currentScanLine += scansToProcess;
			if (m_currentScanLine == GetSize().Height) {
				m_currentChannel++;
				m_currentScanLine = 0;
			}

			if (m_currentChannel >= m_header.Channels || m_currentChannel == 4) {
				// Goddammit, I hate c++ and myself
				RleStream tmpStream;
				m_currentRleStreamData.swap(tmpStream);

				RleScanlineSize tmpSS;
				m_rleScanlineLength.swap(tmpSS);

				return LoadStatus::Finished;
			}
		}
		return LoadStatus::Aborted;
	}

	bool CodecPSD::loadRleStream(IO::FileReader::Ptr file, uint8_t* dest, size_t numBytes) {
		if (m_currentRleStreamIndex >= m_numRleEntries) return false;

		size_t currByte = 0;
		size_t streamPosition = 0;
		size_t currentLength = m_rleScanlineLength[m_currentRleStreamIndex];

		if (file->Read(&m_currentRleStreamData.front(), 1, currentLength) != currentLength) {
			return false;
		}

		while (currByte < numBytes) {
			if ((streamPosition + 1) >= currentLength) {
				return false;
			}

			signed char controlByte = m_currentRleStreamData[streamPosition++];

			if (controlByte >= 0) {
				int toCopy = controlByte + 1;
				if (currByte + toCopy > numBytes) {
					return false;
				}

				if (streamPosition + toCopy > currentLength) {
					return false;
				}

				memcpy(dest, &m_currentRleStreamData.front() + streamPosition, toCopy);

				streamPosition += toCopy;
				currByte += toCopy;
				dest += toCopy;
			}
			else if (controlByte >= -127 && controlByte < 0) {
				int toCopy = 1 - controlByte;
				if (currByte + toCopy > numBytes) {
					return false;
				}

				memset(dest, m_currentRleStreamData[streamPosition++], toCopy);
				currByte += toCopy;
				dest += toCopy;
			}
		}
		m_currentRleStreamIndex++;

		return true;
	}

	bool CodecPSD::loadRawStream(IO::FileReader::Ptr file, uint8_t* dest, size_t numBytes) {
		file->ReadFull(dest, numBytes);
		return true;
	}

	Img::Format CodecPSD::determineSurfaceFormat() {
		switch(m_header.ColorMode) {
			case PsdHeader::ColorModeBitmap:
			case PsdHeader::ColorModeGrayscale:
			case PsdHeader::ColorModeIndexed:
				return Img::Format::Index8;
			case PsdHeader::ColorModeRgb:
				if (m_header.BitsPerChannel != 8) return Img::Format::Undefined;
				if (m_header.Channels >= 4) return Img::Format::ARGB8888;
				if (m_header.Channels == 3) return Img::Format::XRGB8888;
				return Img::Format::Undefined;
		}
		return Img::Format::Undefined;
	}

	uint8_t* CodecPSD::transferRgbScanlinePlane(uint8_t* destination, uint8_t* source, size_t numToTransfer) {
		for (size_t x = 0; x < numToTransfer; ++x) {
			*destination = *source++;
			destination += 4;
		}
		return destination;
	}

	uint8_t* CodecPSD::transferIndexedScanline(uint8_t* destination, uint8_t* source, size_t numToTransfer) {
		memcpy(destination, source, numToTransfer);
		return destination + numToTransfer;
	}

	uint8_t* CodecPSD::transferBitmapScanline(uint8_t* destination, uint8_t* source, size_t numToTransfer) {
		Util::BitsToBytes(destination, source, numToTransfer);
		return destination + numToTransfer;
	}

}
