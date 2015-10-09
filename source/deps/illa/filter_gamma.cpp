#include "filter.h"
#include "filter_int.h"
#include "lut_gamma.h"
#include "config.h"

namespace Filter {
	using namespace Geom;

	static const int GammaZero=64;

	void Adjust::Gamma(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, const int gamma) {
		if (gamma == 10) {
			return;
		}

		if (source.BufferData == nullptr || dest.BufferData == nullptr) {
			DO_THROW(Err::InvalidParam, TX("Null buffer not allowed."));
		}

		if (gamma < Img::MinGamma || gamma > Img::MaxGamma) {
			DO_THROW(Err::InvalidParam, TX("Gamma out of bounds."));
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

		const uint8_t* gamma_table = c_gamma[gamma - Img::MinGamma];

		uint8_t* sourceCurrentScanlinePtr = source.BufferData;
		uint8_t* destCurrentScanlinePtr = dest.BufferData + destPos.X * 4 + destPos.Y * dest.Stride;

		for(int y = y1; y < y2; ++y) {
			uint32_t* sourceCurrentPixelPtr	= reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtr);
			uint32_t* destCurrentPixelPtr		= reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);

			for(int x = x1; x < x2; ++x) {
				uint32_t& scol = *(sourceCurrentPixelPtr++);

				uint8_t newA = static_cast<uint8_t>(Img::ChannelARGB8888Alpha(scol));

				uint8_t newR = gamma_table[Img::ChannelARGB8888Red(scol)];
				uint8_t newG = gamma_table[Img::ChannelARGB8888Green(scol)];
				uint8_t newB = gamma_table[Img::ChannelARGB8888Blue(scol)];

				*(destCurrentPixelPtr++) = Img::ToARGBDWORD(newA, newR, newG, newB);
			}
			sourceCurrentScanlinePtr += source.Stride;
			destCurrentScanlinePtr += dest.Stride;
		}
	}
}
