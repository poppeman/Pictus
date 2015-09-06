#ifndef PSD_HEADER_H
#define PSD_HEADER_H

#include "orz/file_reader.h"

namespace Img {
	namespace Internal {
		class PsdHeader {
		public:
			enum ColorModes {
				ColorModeBitmap = 0,
				ColorModeGrayscale = 1,
				ColorModeIndexed = 2,
				ColorModeRgb = 3,
				ColorModeCMYK = 4,
				ColorModeMultiChannel = 7,
				ColorModeDuotone = 8,
				ColorModeLab = 9
			};

			char Signature[4];
			ColorModes ColorMode;
			uint16_t Channels;
			uint16_t BitsPerChannel;
			uint32_t Height, Width;

			bool ReadHeader(IO::FileReader::Ptr file);
		};
	}
}

#endif
