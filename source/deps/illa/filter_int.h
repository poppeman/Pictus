#ifndef FILTER_INT_H
#define FILTER_INT_H

#include "types.h"

namespace Filter {
	Geom::RectInt CropResampleRegions(const Geom::RectInt& region, const Geom::SizeInt& srcDims, const Geom::SizeInt& dstDims, float zoom);

	namespace Scale {
		void NearestNeighbor(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom);
		void Bilinear(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom);
		void Lanczos3(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom);
		void DirectCopy(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format);
	}
};

#endif
