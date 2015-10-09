#include "filter_buffer.h"

namespace Filter
{
	void FilterBuffer::Construct(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_)
	{
		Dimensions = sz_;
		PixelSize = ps_;
		BufferData = data_;
		Stride = stride_;
	}

	void FilterBuffer::Construct(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal_)
	{
		Dimensions = sz_;
		PixelSize = ps_;
		BufferData = data_;
		Stride = stride_;
		Palette = pal_;
	}

	bool FilterBuffer::operator==(const FilterBuffer& rhs) const
	{
		return (Dimensions == rhs.Dimensions) && (PixelSize == rhs.PixelSize) && (BufferData == rhs.BufferData) && (Stride == rhs.Stride) && (Palette == rhs.Palette);
	}

	bool FilterBuffer::operator!=(const FilterBuffer& rhs) const
	{
		return !(*this == rhs);
	}

	FilterBuffer::FilterBuffer(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_)
	{
		Construct(sz_, ps_, data_, stride_);
	}

	FilterBuffer::FilterBuffer(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal_)
	{
		Construct(sz_, ps_, data_, stride_, pal_);
	}
}
