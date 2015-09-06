#ifndef PCX_HEADER_H
#define PCX_HEADER_H

#include "orz/file_reader.h"
#include "geom.h"
#include "types.h"

namespace Img {
	namespace Internal {
		class PCXHeader {
		public:
			bool Load(IO::FileReader::Ptr file);

			uint8_t Version;
			bool IsCompressed;
			uint8_t PlaneDepth;
			Geom::SizeInt Size;
			uint8_t ColorPlanes;
			unsigned short BytesPerLine;
			uint16_t PaletteType;

			uint8_t TotColorDepth;

			Img::Palette Palette;
		};
	}
}

#endif
