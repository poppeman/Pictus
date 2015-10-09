#include "filter.h"

namespace Filter {
	void Alpha::IndexedColorKey( const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, Img::Format sourceFormat, uint8_t colorKeyIndex ) {
		if (source.BufferData == nullptr || dest.BufferData == nullptr) {
			DO_THROW(Err::InvalidParam, TX("Null buffer not allowed."));
		}

		if (sourceFormat != Img::Format::Index8) {
			DO_THROW(Err::InvalidParam, TX("Surface format must be indexed."));
		}

		//COND_STRICT(source.Palette, Err::InvalidParam, TX("Palette not set."));

		int y1 = std::max(0, sourceRegion.Top());
		int y2 = std::min(source.Dimensions.Height, sourceRegion.Bottom());
		int x1 = std::max(0, sourceRegion.Left());
		int x2 = std::min(source.Dimensions.Width, sourceRegion.Right());

		if ((x2 + destPos.X) > dest.Dimensions.Width) {
			x2 = dest.Dimensions.Width - destPos.X;
		}

		if ((y2 + destPos.Y) > dest.Dimensions.Height) {
			y2 = dest.Dimensions.Height - destPos.Y;
		}

		uint8_t* sourceCurrentScanlinePtr = source.BufferData;
		uint8_t* destCurrentScanlinePtr = dest.BufferData + destPos.X * 4 + destPos.Y * dest.Stride;

		for (int y = y1; y < y2; ++y) {
			uint8_t* sourceCurrentPixelPtr	= reinterpret_cast<uint8_t*>(sourceCurrentScanlinePtr);
			uint32_t* destCurrentPixelPtr	= reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);

			for(int x = x1; x < x2; ++x) {
				uint8_t sp = *(sourceCurrentPixelPtr++);
				if (sp != colorKeyIndex) {
					*destCurrentPixelPtr = Img::ToARGBDWORD(source.Palette.Color(sp));
				}

				destCurrentPixelPtr++;
			}
			sourceCurrentScanlinePtr += source.Stride;
			destCurrentScanlinePtr += dest.Stride;
		}
	}
}
