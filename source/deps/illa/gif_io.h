#ifndef GIF_IO_H
#define GIF_IO_H

#include "color_io.h"

namespace Img {
	void ReadPalette(Img::Palette& pal, int colors, IO::FileReader::Ptr file);
	void ReadSizeWord(Geom::SizeInt* pSize, IO::FileReader::Ptr file);
}

#endif
