#include "filter.h"
#include "filter_int.h"
#include "config.h"

namespace Filter {
	using namespace Geom;

	void Adjust::BrightnessContrast(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, const int newBright, const int newContrast) {
		if (source.BufferData == nullptr || dest.BufferData == nullptr) {
			DO_THROW(Err::InvalidParam, TX("Null buffer not allowed."));
		}

		if (newBright == 0 && newContrast == Img::ContrastStep) {
			return;
		}

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

		float contrastMultiplier = (static_cast<float>(newContrast) / Img::ContrastStep);

		uint8_t* sourceCurrentScanlinePtr = source.BufferData;
		uint8_t* destCurrentScanlinePtr = dest.BufferData + destPos.X * 4 + destPos.Y * dest.Stride;

		for(int y = y1 ; y < y2; ++y) {
			uint32_t* sourceCurrentPixelPtr	= reinterpret_cast<uint32_t*>(sourceCurrentScanlinePtr);
			uint32_t* destCurrentPixelPtr		= reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);

			for(int x = x1; x < x2; ++x) {
				uint32_t& scol = *(sourceCurrentPixelPtr++);

				int newA = Img::ChannelARGB8888Alpha(scol);

				int newR =((Img::ChannelARGB8888Red(scol) + newBright) - 127);
				newR = static_cast<int>(newR * contrastMultiplier) + 127;
				if (newR < 0) {
					newR = 0;
				}
				else if (newR > 255) {
					newR = 255;
				}

				int newG = (Img::ChannelARGB8888Green(scol) + newBright) - 127;
				newG = static_cast<int>(newG * contrastMultiplier) + 127;
				if (newG < 0) {
					newG = 0;
				}
				else if (newG > 255) {
					newG = 255;
				}

				int newB = (Img::ChannelARGB8888Blue(scol) + newBright) - 127;
				newB = static_cast<int>(newB * contrastMultiplier) + 127;
				if (newB < 0) {
					newB = 0;
				}
				else if (newB > 255) {
					newB = 255;
				}

				*(destCurrentPixelPtr++) = Img::ToARGBDWORD(newA, newR, newG, newB);
			}
			sourceCurrentScanlinePtr += source.Stride;
			destCurrentScanlinePtr += dest.Stride;
		}
	}
}
