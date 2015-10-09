#include "bmp_header.h"
#include "color_io.h"

namespace Img {
	namespace Internal {
		void BMPHeader::Load(IO::FileReader::Ptr file) {
			// Get header start
			FileInt pos = file->Position();
			uint32_t biSize;
			file->ReadFull(&biSize, sizeof(biSize));

			// Validate
			if ((biSize != 12) && (biSize != 40) && (biSize != 56) && (biSize != 108) && (biSize != 124)) {
				DO_THROW(Err::InvalidHeader, L"Header size mismatch.");
			}

			// Read width and height
			long Width	= 0;
			long Height	= 0;

			if (biSize == 12) {
				// OS2 format
				file->ReadFull(&Width, 2);
				file->ReadFull(&Height, 2);
			}
			else {
				// Normal format
				file->ReadFull(&Width, 4);
				file->ReadFull(&Height, 4);
			}
			Size.Width	= Width;
			Size.Height	= Height;

			if (Height < 0) {
				FlipVertical	= false;
				Height			= -Height;
			}
			else {
				FlipVertical = true;
			}

			uint16_t planes = 0;
			file->ReadFull(&planes, sizeof(planes));
			if (planes != 1) {
				DO_THROW(Err::InvalidHeader, L"Invalid number of planes; must equal 1.");
			}

			file->ReadFull(&ColorDepth, sizeof(ColorDepth));

			if (biSize == 12) {
				// OS2 format
				Compression	= CompressRGB;
				NumColors	= 0;
			}
			else {
				// Normal format
				file->ReadFull(&Compression, sizeof(Compression));

				// Validate compression modes
				// Monochrome
				if ((ColorDepth == 1) && (Compression != CompressRGB)) {
					DO_THROW(Err::InvalidHeader, L"Monochrome bitmaps must be uncompressed.");
				}

				// 4-bit
				if ((Compression != CompressRGB) && (Compression != CompressRLE4) && (ColorDepth == 4)) {
					DO_THROW(Err::InvalidHeader, L"Invalid compression for 4-bit bitmap format");
				}

				// 8-bit
				if ((Compression != CompressRGB) && (Compression != CompressRLE8) && (ColorDepth == 8)) {
					DO_THROW(Err::InvalidHeader, L"Invalid compression for 8-bit bitmap format");
				}

				// 16-bit
				if ((ColorDepth == 16) && (Compression != CompressRGB) && (Compression != CompressBitfields)) {
					DO_THROW(Err::InvalidHeader, L"Invalid compression for 16-bit bitmap format");
				}
				// 24-bit
				if ((Compression != CompressRGB) && (ColorDepth == 24)) {
					DO_THROW(Err::InvalidHeader, L"Invalid compression for 24-bit bitmap format");
				}

				// 32-bit
				if ((ColorDepth == 32) && (Compression != CompressRGB) && (Compression != CompressBitfields)) {
					DO_THROW(Err::InvalidHeader, L"Invalid compression for 32-bit bitmap format");
				}

				// Skip horizontal/vertical resolution
				file->Seek(sizeof(uint32_t) + sizeof(long) * 2, IO::SeekMethod::Current);

				// Read palette size.
				file->ReadFull(&NumColors, sizeof(NumColors));
				if (Compression == CompressRGB || Compression == CompressBitfields || NumColors > 256) {
					NumColors = 0;
				}

				// Skip number of important colors
				file->Seek(4, IO::SeekMethod::Current);
			}

			// Calculate palette size if none was provided
			if ((NumColors == 0) && (ColorDepth <= 8)) {
				NumColors = (1 << ColorDepth);
			}

			if (Compression == CompressBitfields) {
				// Red actual color masks
				file->ReadFull(&RedMask, 4);
				file->ReadFull(&GreenMask, 4);
				file->ReadFull(&BlueMask, 4);
			}
			else if (biSize == 12) {
				LoadPalette(Palette, *file, 0, NumColors, Img::LoadBGR); // OS2 format
			}
			else if (biSize == 40) {
				LoadPalette(Palette, *file, 0, NumColors, Img::LoadBGRX); // Normal format
			}

			// Calculate matching shifts
			RedShiftRight	= calcShift(RedMask);
			GreenShiftRight	= calcShift(GreenMask);
			BlueShiftRight	= calcShift(BlueMask);

			// Calc amount of padded bytes
			int fileStride;
			if (ColorDepth >= 8) {
				fileStride = (Width * ColorDepth) / 8;
			}
			else {
				fileStride = (7 + Width * ColorDepth) / 8;
			}
			ScanPadding = static_cast<uint8_t>((fileStride % 4) ? (4 - (fileStride % 4)) : 0);

			file->Seek(pos+biSize, IO::SeekMethod::Begin);
		}

		BMPHeader::BMPHeader():
			RedMask{ 0xff0000 },
			GreenMask{ 0xff00 },
			BlueMask{ 0xff }
		{}

		uint32_t BMPHeader::calcShift(uint32_t mask) {
			if (mask == 0) {
				DO_THROW(Err::InvalidHeader, L"BMP header contained invalid color masks.");
			}

			int shift = 0;
			while((mask & 1) == 0) {
				mask >>= 1;
				++shift;
			}
			return shift;
		}
	}
}
