#include "filter.h"

namespace Filter {
	namespace Alpha {
		void Crossblend(const FilterBuffer& a, Geom::PointInt topLeftA, const FilterBuffer& b, Geom::PointInt topLeftB, Geom::SizeInt sourceAreaSize, FilterBuffer& dest, Geom::PointInt& destTopLeft) {
			if (a.BufferData == nullptr || b.BufferData == nullptr || dest.BufferData == nullptr) {
				DO_THROW(Err::InvalidParam, "Null buffer not allowed.");
			}

			uint8_t* sourceCurrentScanlinePtrA = a.BufferData;
			uint8_t* sourceCurrentScanlinePtrB = b.BufferData;
			uint8_t* destCurrentScanlinePtr = dest.BufferData;

			for (int y = 0; y < sourceAreaSize.Height; y++) {
				uint32_t* sourceCurrentPixelPtrA = reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtrA + (y + topLeftA.Y) * a.Stride) + topLeftA.X;
				uint32_t* sourceCurrentPixelPtrB = reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtrB + (y + topLeftB.Y) * b.Stride) + topLeftB.X;
				uint32_t* destCurrentPixelPtr = reinterpret_cast<uint32_t*>(destCurrentScanlinePtr + (y + destTopLeft.Y) * dest.Stride) + destTopLeft.X;

				for (int x = 0; x < sourceAreaSize.Width; x++) {
					uint32_t& pa = *(sourceCurrentPixelPtrA++);
					uint32_t& pb = *(sourceCurrentPixelPtrB++);
					uint8_t	sa = Img::ChannelARGB8888Alpha(pa);

					*(destCurrentPixelPtr++) = Img::MixARGB8888ARGB8888(pb, pa, sa);
				}
			}
		}
	}
}

