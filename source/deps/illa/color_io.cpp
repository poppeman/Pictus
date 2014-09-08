#include "StdAfx.h"
#include "color_io.h"

namespace Img {
	Color LoadRGB(IO::FileReader& file) {
		uint8_t chans[3];
		file.ReadFull(chans, 3);
		return Img::Color(255, chans[0], chans[1], chans[2]);
	}

	Color LoadBGR(IO::FileReader& file) {
		uint8_t chans[3];
		file.ReadFull(chans, 3);
		return Img::Color(255, chans[2], chans[1], chans[0]);
	}

	Color LoadBGRA(IO::FileReader& file) {
		uint8_t chans[4];
		file.ReadFull(chans, 4);
		return Img::Color(chans[3], chans[2], chans[1], chans[0]);
	}

	Color LoadBGRX(IO::FileReader& file) {
		uint8_t chans[4];
		file.ReadFull(chans, 4);
		return Img::Color(255, chans[2], chans[1], chans[0]);
	}
}
