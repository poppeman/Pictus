#include "orz/orz_math.h"
#include "orz/bitconvert.h"
#include "c_bmp.h"
#include "config.h"
#include "surface_locked_area.h"

namespace Img {
	using namespace Geom;
	using namespace Internal;

	bool CodecBMP::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		m_bfh.Load(file);
		m_header.Load(file);

		// Pick surface format
		info.SurfaceFormat = Img::Format::Undefined;

		if ((m_header.ColorDepth == 1) || (m_header.ColorDepth == 4) || (m_header.ColorDepth == 8)) {
			info.SurfaceFormat = Img::Format::Index8;
		}
		else if (m_header.ColorDepth == 16) {
			if (Math::Bit::CountSet(m_header.GreenMask) == 6) {
				info.SurfaceFormat = Img::Format::RGB565;
			}
			else {
				info.SurfaceFormat = Img::Format::XRGB1555;
			}
		}
		else if ((m_header.ColorDepth == 24) || (m_header.ColorDepth == 32)) {
			info.SurfaceFormat = Img::Format::XRGB8888;
		}
		else {
			DO_THROW(Err::InvalidHeader, TX("CodecBMP: BMP unsupported color depth"));
		}

		if (m_header.FlipVertical && ((m_header.Compression == BMPHeader::CompressRGB) || (m_header.Compression == BMPHeader::CompressBitfields))) {
			m_currScan = m_header.Size.Height;
		}
		else {
			m_currScan = 0;
		}

		m_currPix = 0;

		file->Seek(m_bfh.offset, IO::SeekMethod::Begin);
		m_fileStream.SetFileReader(file);
		m_isValid = true;

		info.Dimensions = m_header.Size;

		return true;
	}

	AbstractCodec::AllocationStatus CodecBMP::PerformAllocate() {
		if (!m_isValid) {
			return AllocationStatus::Failed;
		}

		GetSurface()->CreateSurface(GetSize(), GetFormat());
		if (GetFormat() == Format::Index8) {
			GetSurface()->SetPalette(m_header.Palette);
		}
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecBMP::PerformLoadImageData(IO::FileReader::Ptr file) {
		while (DoTerminate() == false) {
			int startY, nRows;

			if (m_header.FlipVertical) {
				nRows	= std::min<int>(ChunkRows, m_currScan);
				startY	= m_currScan - nRows;
			}
			else {
				nRows	= std::min<long>(ChunkRows, m_header.Size.Height - m_currScan);
				startY	= m_currScan;
			}

			std::shared_ptr<Surface::LockedArea> area;

			if ((m_header.Compression == BMPHeader::CompressBitfields) || (m_header.Compression == BMPHeader::CompressRGB)) {
				area = GetSurface()->LockSurface(RectInt(PointInt(0, startY), SizeInt(GetSize().Width, nRows)));
			}
			else {
				area = GetSurface()->LockSurface(RectInt(PointInt(0, 0), GetSize()));
			}

			if(area == 0) return LoadStatus::Finished;

			ptrdiff_t linestep	= ((m_header.FlipVertical ? -1 : 1) * area->Stride());
			ptrdiff_t yofs		= (m_header.FlipVertical ? ((nRows - 1) * area->Stride()) : 0);

			uint8_t* m_pData = area->Buffer();
			bool isFinished = false;

			switch(m_header.Compression) {
				case BMPHeader::CompressBitfields:
				case BMPHeader::CompressRGB:
					isFinished = decompressRawRgb(nRows, file, m_pData, yofs, linestep);
					break;

				case BMPHeader::CompressRLE8:
					isFinished = decompressRle8(area);
					break;

				case BMPHeader::CompressRLE4:
					isFinished = decompressRle4(file, area);
					break;
			}

			area->Unlock();

			if (isFinished) {
				return LoadStatus::Finished;
			}
		}

		return LoadStatus::Aborted;
	}

	CodecBMP::CodecBMP():
		m_isValid(false)
	{}

	inline void CodecBMP::AddPixelRLE(Surface::LockedArea::Ptr area, uint8_t val) {
		if (m_currPix != GetSize().Width) {
			ptrdiff_t yofs = m_header.FlipVertical
				? ((GetSize().Height - m_currScan - 1) * area->Stride())
				: (m_currScan * area->Stride());

			area->Buffer()[yofs + m_currPix] = val;
			++m_currPix;
		}
	}

	bool CodecBMP::decompressRle4( IO::FileReader::Ptr file, Surface::LockedArea::Ptr area ) {
		for(int chunk = 0; chunk < RLEChunks; chunk++) {
			uint8_t b1 = m_fileStream.ReadByte();
			uint8_t b2 = m_fileStream.ReadByte();

			if (b1 == 0) {
				// Special mode
				switch(b2) {
				case RLE_EOL:		// Switch to next line
					if (m_currPix != 0) {
						++m_currScan;
						m_currPix = 0;
					}
					break;
				case RLE_EOB:		// Done!
					return true;
				case RLE_DELTA:		// Change position
					m_currPix	+= m_fileStream.ReadByte();
					m_currScan	+= m_fileStream.ReadByte();
					break;
				default:			// Absolute mode run
					int numBytes = b2 >> 1;
					for (int i = 0; i < numBytes; ++i) {
						uint8_t pix = m_fileStream.ReadByte();
						AddPixelRLE(area, (pix & 0xf0) >> 4);
						AddPixelRLE(area, (pix & 0x0f));
					}
					if (b2 & 0x1) {
						AddPixelRLE(area, (m_fileStream.ReadByte() & 0xf0) >> 4);
					}
					if ((numBytes + (b2 & 0x01)) & 0x1) {
						m_fileStream.ReadByte();
					}
					break;
				}
			}
			else {
				// Toss in RLE pixels
				int numBytes = b1 >> 1;
				uint8_t pix1 = (b2 & 0xf0) >> 4;
				uint8_t pix2 = b2 & 0xf;
				for(uint8_t i = 0; i < numBytes; ++i) {
					AddPixelRLE(area, pix1);
					AddPixelRLE(area, pix2);
				}
				if (b1 & 0x01) {
					AddPixelRLE(area, pix1);
				}
			}
		}
		return false;
	}

	bool CodecBMP::decompressRle8( Surface::LockedArea::Ptr area ) {
		for(int chunk = 0; chunk < RLEChunks; ++chunk) {
			uint8_t b1 = m_fileStream.ReadByte();
			uint8_t b2 = m_fileStream.ReadByte();

			if (b1 == 0) {
				// Special mode
				switch(b2) {
				case RLE_EOL:		// Switch to next line
					if (m_currPix != 0) {
						m_currScan++;
						m_currPix=0;
					}
					break;
				case RLE_EOB:		// Done!
					return true;
				case RLE_DELTA:		// Change position
					m_currPix	+= m_fileStream.ReadByte(); 
					m_currScan	+= m_fileStream.ReadByte(); 
					break;
				default:			// Absolute mode run
					for (uint8_t i = 0; i < b2; ++i)
						AddPixelRLE(area, m_fileStream.ReadByte());

					if (b2 & 0x1) m_fileStream.ReadByte();
					break;
				}
			}
			else {
				for (uint8_t i = 0; i < b1; ++i) {
					AddPixelRLE(area, b2);
				}
			}
		}
		return false;
	}

	bool CodecBMP::decompressRawRgb( int nRows, IO::FileReader::Ptr file, uint8_t* m_pData, ptrdiff_t &yofs, ptrdiff_t linestep ) {
		if (m_header.ColorDepth < 8) {
			yofs = decompressRawIndex4_1(nRows, file, m_pData, yofs, linestep);
		}
		else if (m_header.ColorDepth == 8) {
			yofs = decompressRawIndex8(nRows, file, m_pData, yofs, linestep);
		}
		else if ((m_header.ColorDepth == 32) || (m_header.ColorDepth == 24)) {
			yofs = decompressRawRgb24_32(nRows, m_pData, yofs, linestep);
		}
		else if (m_header.ColorDepth == 16) {
			yofs = decompressRawRgb16(nRows, m_pData, yofs, linestep);
		}

		if (m_header.FlipVertical) {
			m_currScan -= nRows;
		}
		else {
			m_currScan += nRows;
		}

		if (((m_currScan == GetSize().Height) && (m_header.FlipVertical == false)) || ((m_currScan == 0) && m_header.FlipVertical)) {
			return true;
		}

		return false;
	}

	ptrdiff_t CodecBMP::decompressRawRgb16( int nRows, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep ) {
		for(int currY = 0; currY < nRows; ++currY) {
			uint8_t* pData8 = m_pData + yofs;

			if (Math::Bit::CountSet(m_header.GreenMask) == 6) {
				// No need to set alpha bit.
				for(int currX = 0; currX < GetSize().Width; ++currX) {
					*pData8++ = m_fileStream.ReadByte();
					*pData8++ = m_fileStream.ReadByte();
				}
			}
			else {
				// Set alpha bit.
				for(int currX = 0; currX < GetSize().Width; ++currX) {
					*pData8++ = m_fileStream.ReadByte();
					*pData8++ = m_fileStream.ReadByte() | 0x80;
				}
			}
			yofs += linestep;
			m_fileStream.SkipBytes(m_header.ScanPadding);
		}
		return yofs;
	}

	ptrdiff_t CodecBMP::decompressRawRgb24_32( int nRows, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep ) {
		int bytesPerPixel = m_header.ColorDepth / 8;

		for(int currY = 0; currY < nRows; currY++) {
			// Prettier to use 32-bit arrays for 32-bit buffers
			uint32_t* pData32 = reinterpret_cast<uint32_t*>(m_pData + yofs);
			for(int currX = 0; currX < GetSize().Width; currX++) {
				uint32_t currentPixel = 0;
				uint8_t* pByteCurrentPixel = reinterpret_cast<uint8_t*>(&currentPixel);
				for (int b = 0; b < bytesPerPixel; b++) {
					pByteCurrentPixel[b] = m_fileStream.ReadByte();
				}

				// Convert pixel data and store in the surface
				*(pData32++)=
					0xff000000															+	// Alpha
					(((currentPixel & m_header.RedMask)		>> m_header.RedShiftRight)	<< 16)	+	// Red
					(((currentPixel & m_header.GreenMask)	>> m_header.GreenShiftRight)<< 8)	+	// Green
					(((currentPixel & m_header.BlueMask)	>> m_header.BlueShiftRight));			// Blue
			}
			yofs += linestep;
			m_fileStream.SkipBytes(m_header.ScanPadding);
		}
		return yofs;
	}

	ptrdiff_t CodecBMP::decompressRawIndex8( int nRows, IO::FileReader::Ptr file, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep ) {
		for(int currY = 0; currY < nRows; currY++) {
			file->ReadFull(&m_pData[yofs], GetSize().Width);
			yofs += linestep;
			file->Seek(m_header.ScanPadding, IO::SeekMethod::Current);
		}
		return yofs;
	}

	ptrdiff_t CodecBMP::decompressRawIndex4_1( int nRows, IO::FileReader::Ptr file, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep ) {
		int pixelsPerByte	= 8 / m_header.ColorDepth;
		int totalBytesPerRow = (GetSize().Width + pixelsPerByte - 1) / pixelsPerByte;

		auto currScanSource = std::make_unique<uint8_t[]>(totalBytesPerRow);

		for(int currY = 0; currY < nRows; currY++) {
			file->ReadFull(currScanSource.get(), totalBytesPerRow);
			if (m_header.ColorDepth == 1) {
				Util::BitsToBytes(&m_pData[yofs], currScanSource.get(), GetSize().Width);
			}
			else if (m_header.ColorDepth == 4) {
				Util::NibblesToBytes(&m_pData[yofs], currScanSource.get(), GetSize().Width);
			}

			yofs += linestep;
			file->Seek(m_header.ScanPadding, IO::SeekMethod::Current);
		}
		return yofs;
	}
}
