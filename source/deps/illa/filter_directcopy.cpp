#include "StdAfx.h"

#include "filter_int.h"

namespace Filter {
	namespace Scale {
		void DirectCopy(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format) {
			int pixelSize = Img::EstimatePixelSize(format);

			int width = region.Width();
			int scans = region.Height();

			size_t srcstride = source.Stride;
			size_t dststride = dest.Stride;

			if ((format == Img::Format::XRGB8888) || (format == Img::Format::ARGB8888)) {
				ptrdiff_t srcPos	= region.Top() * srcstride + region.Left() * pixelSize;
				ptrdiff_t dstPos	= 0;

				for(int i = 0; i < scans; ++i) {
					memcpy(dest.BufferData + dstPos, source.BufferData + srcPos, width * pixelSize);
					srcPos += srcstride;
					dstPos += dststride;
				}
			}
			else if ((format == Img::Format::ARGB1555) || (format == Img::Format::XRGB1555)) {
				ptrdiff_t srcPos	= region.Top() * (srcstride / 2) + region.Left();
				ptrdiff_t dstPos	= 0;

				uint32_t*	pDst = reinterpret_cast<uint32_t*>(dest.BufferData);
				uint16_t*	pSrc = reinterpret_cast<uint16_t*>(source.BufferData);

				ptrdiff_t sldiff = srcstride / 2 - width;
				ptrdiff_t dldiff = dststride / 4 - width;

				for(int i = 0; i < scans; ++i) {
					for(int x = 0; x < width; ++x) {
						pDst[dstPos] = Img::ConvertARGB1555ToARGB8888(pSrc[srcPos]);
						srcPos++;
						dstPos++;
					}
					srcPos += sldiff;
					dstPos += dldiff;
				}
			}
			else if (format == Img::Format::RGB565) {
				ptrdiff_t srcPos	= region.Top() * (srcstride / 2) + region.Left();
				ptrdiff_t dstPos	= 0;

				uint32_t*	pDst = reinterpret_cast<uint32_t*>(dest.BufferData);
				uint16_t*	pSrc = reinterpret_cast<uint16_t*>(source.BufferData);

				ptrdiff_t sldiff	= srcstride / 2 - width;
				ptrdiff_t dldiff	= dststride / 4 - width;

				for(int i = 0; i < scans; ++i) {
					for(int x = 0; x < width; ++x) {
						pDst[dstPos] = Img::ConvertRGB565ToARGB8888(pSrc[srcPos]);
						srcPos++;
						dstPos++;
					}
					srcPos += sldiff;
					dstPos += dldiff;
				}
			}
			else if (format == Img::Format::Index8) {
				ptrdiff_t srcPos	= region.Top() * srcstride + region.Left();
				ptrdiff_t dstPos	= 0;

				uint32_t*	pDst = reinterpret_cast<uint32_t*>(dest.BufferData);
				uint8_t*	pSrc = reinterpret_cast<uint8_t*>(source.BufferData);

				ptrdiff_t sldiff	= srcstride - width;
				ptrdiff_t dldiff	= dststride / 4 - width;

				for(int i = 0; i < scans; ++i) {
					for(int x = 0; x < width; ++x) {
						pDst[dstPos++] = Img::ToARGBDWORD(source.Palette.Color(pSrc[srcPos]));
						srcPos++;
					}
					srcPos += sldiff;
					dstPos += dldiff;
				}
			}
			else
				DO_THROW(Err::InvalidParam, TX("Format not supported:") + ToWString(format));
		}
	}
}
