#ifndef BMP_HEADER_H
#define BMP_HEADER_H

#include "orz/file.h"
#include "types.h"

namespace Img {
	namespace Internal {
		class BMPHeader {
		public:
			enum Compress {
				CompressRGB = 0,
				CompressRLE8 = 1,
				CompressRLE4 = 2,
				CompressBitfields = 3,
			};

			void Load(IO::FileReader::Ptr file);
			BMPHeader();

			Geom::SizeInt Size;
			uint16_t ColorDepth;
			bool FlipVertical;
			uint32_t Compression;
			int NumColors;

			uint32_t RedMask;
			uint32_t GreenMask;
			uint32_t BlueMask;

			uint32_t RedShiftRight;
			uint32_t GreenShiftRight;
			uint32_t BlueShiftRight;

			uint8_t ScanPadding;

			Img::Palette Palette;

		private:
			uint32_t calcShift(uint32_t mask);
		};
	}
}

#endif
