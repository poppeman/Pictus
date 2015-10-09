#include "bmp_fileheader.h"
#include "types.h"

namespace Img {
	namespace Internal {
		void BMPFileHeader::Load(IO::FileReader::Ptr file) {
			enum {
				BITMAP_SIGN = 0x4D42,
			};

			file->ReadFull(&type, 2);
			file->ReadFull(&size, 4);
			file->Seek(4, IO::SeekMethod::Current);	// Skip reserved bytes
			file->ReadFull(&offset, 4);

			// Validate
			if (type != BITMAP_SIGN) {
				DO_THROW(Err::InvalidHeader, "Magic bytes didn't match.");
			}
		}

		BMPFileHeader::BMPFileHeader() {}
	}
}
