#ifndef C_TGA_HEADER_H
#define C_TGA_HEADER_H

#include "orz/file.h"
#include "types.h"

namespace Img {
	class TGAHeader {
	public:
		enum TGAImageType {
			NoData = 0,				// No image data present.
			RawColormap = 1,		// RAW color-mapped data.
			RawTrueColor = 2,		// RAW true-color data.
			RawBW = 3,				// RAW gray-scale only data.
			RLEColormap = 9,		// RLE color mapped data.
			RLETrueColor = 10,		// RLE true-color data.
			RLEBW = 11,				// RLE gray-scale only data.
		};
		enum AtributesType {
			AT_NoAlpha = 0,
			AT_UndefinedIgnore = 1,
			AT_UndefinedRetain = 2,
			AT_Alpha = 3,
			AT_AlphaPreMultiplied = 4,
		};

		uint8_t IdSize;
		uint8_t ColorMapType;
		TGAImageType ImageType;

		unsigned short ColorMapStart;
		unsigned short ColorMapLength;
		uint8_t ColorMapBits;

		Geom::SizeInt Size;

		uint8_t ColorDepth;
		uint8_t AlphaBits;
		AtributesType AttributesType;

		bool FlipHorizontal;
		bool FlipVertical;

		bool LoadHeader(IO::FileReader::Ptr file);

		Img::Format SurfaceFormat();

		TGAHeader();

	private:
		bool ReadExtensionArea(IO::FileReader::Ptr file);
	};
}

#endif
