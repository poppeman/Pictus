#include "filter.h"

namespace Filter {
	namespace Alpha {
		void SolidColor(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, const Img::Color& bgr) {
			if (source.BufferData == nullptr || dest.BufferData == nullptr) {
				DO_THROW(Err::InvalidParam, L"Null buffer not allowed.");
			}

			int y1 = Util::Max(0, sourceRegion.Top());
			int y2 = Util::Min(source.Dimensions.Height, sourceRegion.Bottom());
			int x1 = Util::Max(0, sourceRegion.Left());
			int x2 = Util::Min(source.Dimensions.Width, sourceRegion.Right());

			if ((x2 + destPos.X) > dest.Dimensions.Width) {
				x2 = dest.Dimensions.Width - destPos.X;
			}

			if ((y2 + destPos.Y) > dest.Dimensions.Height) {
				y2 = dest.Dimensions.Height - destPos.Y;
			}

			uint32_t bgrDW	= Img::ToARGBDWORD(bgr);

			uint8_t* sourceCurrentScanlinePtr = source.BufferData;
			uint8_t* destCurrentScanlinePtr = dest.BufferData + destPos.X * 4 + destPos.Y * dest.Stride;

			for (int y = y1; y < y2; ++y) {
				uint32_t* sourceCurrentPixelPtr = reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtr);
				uint32_t* destCurrentPixelPtr = reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);

				for(int x = x1; x < x2; ++x) {
					uint32_t&	sp = *(sourceCurrentPixelPtr++);
					uint8_t	sa = Img::ChannelARGB8888Alpha(sp);

					*(destCurrentPixelPtr++) = Img::MixARGB8888ARGB8888(bgrDW, sp, sa);
				}
				sourceCurrentScanlinePtr += source.Stride;
				destCurrentScanlinePtr += dest.Stride;
			}
		}
	}
}
