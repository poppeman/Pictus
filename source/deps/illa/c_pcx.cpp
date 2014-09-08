#include "StdAfx.h"

#include "orz/bitconvert.h"
#include "c_pcx.h"
#include "surface.h"
#include "surfacemgr.h"
#include "color_io.h"

namespace Img {
	using namespace Geom;

	bool CodecPCX::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		if (m_header.Load(file) == false) {
			return false;
		}

		info.SurfaceFormat = determineSurfaceFormat(m_header.TotColorDepth);
		if (info.SurfaceFormat == Img::Format::Undefined) {
			return false;
		}

		m_cache.SetFileReader(file);
		m_currScan = 0;
		info.Dimensions = m_header.Size;

		return true;
	}

	AbstractCodec::AllocationStatus CodecPCX::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());
		if (GetFormat() == Format::Index8) {
			GetSurface()->SetPalette(m_header.Palette);
		}

		GetSurface()->ClearSurface(Color::FromDWord(0));
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecPCX::PerformLoadImageData( IO::FileReader::Ptr file ) {
		while (DoTerminate() == false) {
			int nRows	= Util::Min<int>(ChunkRows, GetSize().Height - m_currScan);
			int startY	= m_currScan;

			Surface::LockedArea::Ptr area = GetSurface()->LockSurface(RectInt(PointInt(0, startY), SizeInt(GetSize().Width, nRows)));
			uint8_t* blinep	= area->Buffer();
			size_t stride	= area->Stride();

			// Allocate a temporary scan line buffer
			size_t scanBytes = m_header.ColorPlanes * m_header.BytesPerLine;
			auto currData = std::make_unique<uint8_t[]>(scanBytes);

			size_t yofs = 0;

			for(int currY = 0; currY < nRows; ++currY) {
				// Load the scan line
				uint8_t* p = currData.get();

				if (m_header.IsCompressed == true) {
					DecodeScanline(file, p, scanBytes);
				}
				else {
					if (file->Read(p, scanBytes, 1) != 1) {
						return LoadStatus::Failed;
					}
				}

				if (m_header.ColorPlanes != 1) {
					transferPlanar(blinep + yofs, p);
				}
				else {
					transferNonPlanar(blinep + yofs, p);
				}

				yofs += stride;
			}

			m_currScan += nRows;

			area->Unlock();

			if (m_currScan == GetSize().Height) {
				return LoadStatus::Finished;
			}
		}
		return LoadStatus::Aborted;
	}

	void CodecPCX::DecodeScanline(IO::FileReader::Ptr file, uint8_t* dest, size_t maxBytes) {
		int currByte = 0;
		int numBytes = m_header.ColorPlanes * m_header.BytesPerLine;
		int bytesWritten = 0;

		while(currByte < numBytes) {
			uint8_t count, data;
			if (m_cache.BytesLeft() == 0) {
				return;
			}

			count = m_cache.ReadByte();
			if (count >= 192) {
				if (m_cache.BytesLeft() == 0) {
					return;
				}

				// RLE counter, read data and adjust count
				count -= 192;
				data = m_cache.ReadByte();
			}
			else {
				// Raw value, count=1
				data	= count;
				count	= 1;
			}

			// Feed the bytes into the stream
			if (count + bytesWritten > maxBytes) {
				count = maxBytes - bytesWritten;
			}
			memset(dest, data, count);

			bytesWritten += count;

			dest += count;
			currByte += count;
		}
	}

	Img::Format CodecPCX::determineSurfaceFormat( uint8_t colorDepth ) {
		if (colorDepth <= 8) {
			return Img::Format::Index8;
		}
		if (colorDepth == 16) {
			return Img::Format::RGB565;
		}
		if (colorDepth == 24) {
			return Img::Format::XRGB8888;
		}

		return Img::Format::Undefined;
	}

	void CodecPCX::transferNonPlanar(uint8_t* destination, uint8_t* source) {
		if (m_header.PlaneDepth == 1) {
			Util::BitsToBytes(destination, source, GetSize().Width);
		}
		else if (m_header.PlaneDepth == 4) {
			Util::NibblesToBytes(destination, source, GetSize().Width);
		}
		else if (m_header.PlaneDepth == 8) {
			memcpy(destination, source, GetSize().Width);
		}
		else {
			DO_THROW(Err::CriticalError, TX("Non-planar with depth:") + ToWString(m_header.PlaneDepth));
		}
	}

	void CodecPCX::transferPlanar( uint8_t* destination, uint8_t* source ) {
		if (m_header.ColorPlanes == 4) {
			transferPlanar4(destination, source);
		}
		else if (m_header.ColorPlanes == 3 && m_header.PlaneDepth == 8) {
			transferPlanar38(destination, source);
		}
	}

	void CodecPCX::transferPlanar4( uint8_t* destination, uint8_t* source) {
		for(uint8_t plane = 0; plane < 4; plane++) {
			uint8_t pixbit	= static_cast<uint8_t>(1 << plane);
			int ofs			= 0;

			for(int currX = 0; currX < m_header.BytesPerLine; currX++)  {
				uint8_t w = *source++;

				for(uint8_t mask = 0x80; mask != 0; mask >>= 1) {
					if (w & mask) {
						destination[ofs] |= pixbit;
					}

					ofs++;
				}
			}
		}
	}

	void CodecPCX::transferPlanar38( uint8_t* destination, uint8_t* source ) {
		for(int plane = 0; plane < 3; plane++) {
			size_t total_ofs = 0;
			uint8_t *pc = source  + m_header.BytesPerLine * plane;

			for(int currX = 0; currX < GetSize().Width; currX++) {
				destination[total_ofs + 4 - 1] = 255;
				destination[total_ofs + 4 - plane-2] = *pc++;

				total_ofs += 4;
			}
		}
	}
}
