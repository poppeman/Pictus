#ifndef COLOR_IO_H
#define COLOR_IO_H

#include "orz/file_reader.h"
#include "types.h"

namespace Img {
	Img::Color LoadRGB(IO::FileReader& file);
	Img::Color LoadBGR(IO::FileReader& file);
	Img::Color LoadBGRA(IO::FileReader& file);
	Img::Color LoadBGRX(IO::FileReader& file);

	template <class Fn>
	void LoadPalette(Img::Palette& pal, IO::FileReader& file, unsigned firstEntry, unsigned numEntries, Fn func) {
		if ((firstEntry + numEntries) > 256) {
			DO_THROW(Err::InvalidParam, L"Color count out of bounds (> 255)");
		}

		for(unsigned i = 0; i < numEntries; i++) {
			pal.Color(firstEntry + i, func(file));
		}
	}
}

#endif
