#ifndef ILLA_FILTER_BUFFER_H
#define ILLA_FILTER_BUFFER_H

#include "orz/geom.h"
#include "palette.h"

namespace Filter
{
	struct FilterBuffer
	{
		Geom::SizeInt Dimensions;
		int PixelSize;
		uint8_t* BufferData;
		size_t Stride;
		Img::Palette Palette;

		void Construct(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_);
		void Construct(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal_);

		bool operator==(const FilterBuffer& rhs) const;
		bool operator!=(const FilterBuffer& rhs) const;

		FilterBuffer(const Geom::SizeInt& sz_ = Geom::SizeInt(0, 0), int ps_ = 0, uint8_t* data_ = 0, size_t stride_ = 0);
		FilterBuffer(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal);
	};
}

#endif
