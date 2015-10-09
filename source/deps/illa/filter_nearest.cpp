#include "types.h"

namespace Filter {
	namespace Scale {
		using namespace Geom;

		struct ConvertAXRGB8888 {
			uint32_t operator()(const uint32_t& v) const { return v; }
		};

		struct ConvertARGB1555 {
			uint32_t operator()(const uint16_t& v) const { return Img::ConvertARGB1555ToARGB8888(v); }
		};

		struct ConvertXRGB1555 {
			uint32_t operator()(const uint16_t& v) const { return Img::ConvertXRGB1555ToARGB8888(v); }
		};

		struct ConvertRGB565 {
			uint32_t operator()(const uint16_t& v) const { return Img::ConvertRGB565ToARGB8888(v); }
		};

		struct ConvertIndexed {
			ConvertIndexed(const Img::Palette& p):m_p(p) {}
			uint32_t operator()(const uint8_t& v) const { return Img::ToARGBDWORD(m_p.Color(v)); }
			const Img::Palette& m_p;

			void operator=(const ConvertIndexed&) = delete;
		};

		template <typename T, class U, int SHIFT, int MUL>
		inline void performNearestResampleWithGivenPrecision(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, float zoom, U& pixelConverter) {
			int xSD = static_cast<int>(MUL / zoom);
			int xSstart	= xSD * region.Left();

			uint8_t* destCurrentScanlinePtr = dest.BufferData;
			int rh = region.Height();
			int rw = region.Width();

			for (int y = 0; y < rh; ++y) {
				const T* currSourceScan = reinterpret_cast<T*>(source.BufferData + (static_cast<size_t>((region.Top() + y) / zoom)) * source.Stride);
				uint32_t* currDestPixel = reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);

				size_t xSofs = xSstart;

				for (int x = 0; x < rw; ++x) {
					*(currDestPixel++) = pixelConverter(currSourceScan[xSofs >> SHIFT]);
					xSofs += xSD;
				}
				destCurrentScanlinePtr += dest.Stride;
			}
		}

		template <typename T, class U>
		inline void performNearestResample(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, float zoom, U pixelConverter) {
			if (zoom > 10.0)
				performNearestResampleWithGivenPrecision<T, U, 18, 262144>(source, dest, region, zoom, pixelConverter);
			else
				performNearestResampleWithGivenPrecision<T, U, 11, 2048>(source, dest, region, zoom, pixelConverter);
		}

		void NearestNeighbor(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom) {
			switch(format) {
				case Img::Format::ARGB8888:
				case Img::Format::XRGB8888:
					performNearestResample<uint32_t>(source, dest, region, zoom, ConvertAXRGB8888());
					break;
				case Img::Format::ARGB1555:
					performNearestResample<uint16_t>(source, dest, region, zoom, ConvertARGB1555());
					break;
				case Img::Format::XRGB1555:
					performNearestResample<uint16_t>(source, dest, region, zoom, ConvertXRGB1555());
					break;
				case Img::Format::RGB565:
					performNearestResample<uint16_t>(source, dest, region, zoom, ConvertRGB565());
					break;
				case Img::Format::Index8:
					performNearestResample<uint8_t>(source, dest, region, zoom, ConvertIndexed(source.Palette));
					break;
				default:
					DO_THROW(Err::InvalidParam, L"Surface format not supported:" + ToWString(format));
			}
		}
	}
}
