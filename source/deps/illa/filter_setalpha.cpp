#include "filter.h"

namespace Filter {
	namespace Alpha {
		void SetAlpha(const FilterBuffer& source, int alpha) {
			if (source.BufferData == nullptr) {
				DO_THROW(Err::InvalidParam, "Null buffer not allowed.");
			}

			uint8_t* sourceCurrentScanlinePtr = source.BufferData;
			uint32_t alphaComponent = alpha << 24;

			for (int y = 0;y < source.Dimensions.Height; ++y) {
				uint32_t* sourceCurrentPixelPtr = reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtr);

				for(int x = 0; x < source.Dimensions.Width; ++x) {
					uint32_t&	sp = *sourceCurrentPixelPtr;
					*(sourceCurrentPixelPtr++) = (sp & 0x00ffffff) + alphaComponent;
				}
				sourceCurrentScanlinePtr += source.Stride;
			}
		}
	}
}
