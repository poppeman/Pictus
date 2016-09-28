#include "c_tga_header.h"
#include <cstring>

namespace Img {
	bool TGAHeader::LoadHeader(IO::FileReader::Ptr file) {
		if (file->Size() - file->Position() < 19) {
			return false;
		}

		file->ReadFull(&IdSize, 1);
		file->ReadFull(&ColorMapType, 1);
		file->ReadFull(&ImageType, 1);
		file->ReadFull(&ColorMapStart, 2);
		file->ReadFull(&ColorMapLength, 2);
		file->ReadFull(&ColorMapBits, 1);

		// Skip X/Y start coordinates
		file->Seek(4, IO::SeekMethod::Current);
		file->ReadFull(&Size.Width, 2);
		file->ReadFull(&Size.Height, 2);
		file->ReadFull(&ColorDepth, 1);

		// 00vhaaaa (vertical flip, horizontal flip, alpha bits)
		uint8_t desc;
		file->ReadFull(&desc, 1);

		// Parse the desc-byte
		AlphaBits		= desc & 0x0f;
		FlipHorizontal	= ((desc & 0x10) != 0);
		FlipVertical	= ((desc & 0x20) == 0);

		if (AlphaBits) {
			AttributesType = AT_Alpha;
		}

		FileInt headerEnd = file->Position();

		file->Seek(file->Size() - 26, IO::SeekMethod::Begin);
		char footer[26];
		file->ReadFull(footer, 26);
		if (memcmp(footer + 8, "TRUEVISION-XFILE", 16) == 0) {
			AttributesType = AT_UndefinedIgnore;

			// New TGA format. See if there's an extension area
			file->Seek(file->Size() - 26, IO::SeekMethod::Begin);
			uint32_t offset = 0;
			file->ReadFull(&offset, 4);
			if (offset != 0) {
				file->Seek(offset, IO::SeekMethod::Begin);
				if (!ReadExtensionArea(file)) {
					return false;
				}
			}
		}

		file->Seek(headerEnd, IO::SeekMethod::Begin);

		return true;
	}

	bool TGAHeader::ReadExtensionArea( IO::FileReader::Ptr file ) {
		uint16_t size;
		file->ReadFull(&size, 2);
		if (size < 495) return false;

		// Skip tons of worthless crap
		file->Seek(492, IO::SeekMethod::Current);
		AttributesType = AT_NoAlpha;
		file->ReadFull(&AttributesType, 1);
		
		// HACK: Some software writes NoAlpha even though alpha-bits are present
		if (AttributesType == AT_NoAlpha && AlphaBits) {
			AttributesType = AT_Alpha;
		}

		return true;
	}

	Img::Format TGAHeader::SurfaceFormat() {
		if (ColorDepth == 8) {
			return Img::Format::Index8;
		}

		if (ColorDepth == 16) {
			if (AttributesType == AT_Alpha) {
				return Img::Format::ARGB1555;
			}
			else {
				return Img::Format::XRGB1555;
			}
		}

		if (ColorDepth == 24) {
			return Img::Format::XRGB8888;
		}

		if (ColorDepth == 32) {
			if (AttributesType == AT_Alpha) {
				return Img::Format::ARGB8888;
			}
			else {
				return Img::Format::XRGB8888;
			}
		}

		return Img::Format::Undefined;
	}
}
