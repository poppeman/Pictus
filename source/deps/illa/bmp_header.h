#ifndef BMP_HEADER_H
#define BMP_HEADER_H

#include "orz/file_reader.h"
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
			uint16_t ColorDepth = 0;
			bool FlipVertical = false;
			uint32_t Compression = CompressRGB;
			uint32_t NumColors = 0;

			uint32_t RedMask = 0xff0000;
			uint32_t GreenMask = 0xff00;
			uint32_t BlueMask = 0xff;

			uint32_t RedShiftRight = 0;
			uint32_t GreenShiftRight = 0;
			uint32_t BlueShiftRight = 0;

			uint8_t ScanPadding = 0;

			Img::Palette Palette;

		private:
			uint32_t calcShift(uint32_t mask);
		};
	}
}

#endif
