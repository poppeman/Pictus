#include "filter.h"

namespace Filter {
	namespace Alpha {
		void PremultiplyAlphaBuffer(const FilterBuffer& source) {
			if (source.BufferData == nullptr) {
				DO_THROW(Err::InvalidParam, L"Null buffer not allowed.");
			}

			uint8_t* sourceCurrentScanlinePtr = source.BufferData;

			for (int y = 0;y < source.Dimensions.Height; ++y) {
				uint32_t* sourceCurrentPixelPtr = reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtr);

				for(int x = 0; x < source.Dimensions.Width; ++x) {
					uint32_t&	sp = *sourceCurrentPixelPtr;
					uint8_t	sa = Img::ChannelARGB8888Alpha(sp);

					*(sourceCurrentPixelPtr++) = Img::ToARGBDWORD(
						sa,
						(Img::ChannelARGB8888Red(sp) * sa) / 0xff,
						(Img::ChannelARGB8888Green(sp) * sa) / 0xff,
						(Img::ChannelARGB8888Blue(sp) * sa) / 0xff);
				}
				sourceCurrentScanlinePtr += source.Stride;
			}
		}
	}
}
