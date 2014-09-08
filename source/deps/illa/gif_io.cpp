#include "StdAfx.h"
#include "gif_io.h"

namespace Img {
	void ReadPalette(Img::Palette& pal, int colors, IO::FileReader::Ptr file) {
		if (file == nullptr) {
			DO_THROW(Err::InvalidParam, TX("file was null"));
		}
		LoadPalette(pal, *file, 0, colors, Img::LoadRGB);
	}

	void ReadSizeWord(Geom::SizeInt* pSize, IO::FileReader::Ptr file) {
		if (file == nullptr) {
			DO_THROW(Err::InvalidParam, TX("file was null"));
		}

		uint16_t sizes[2];
		file->ReadFull(sizes, 2 * 2);
		pSize->Width	= sizes[0];
		pSize->Height	= sizes[1];
	}
}
