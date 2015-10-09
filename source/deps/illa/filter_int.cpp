#include "filter_int.h"

namespace Filter {
	int crop_value(int value, float zoom, int cap) {
		if (static_cast<int>(0.5f + value / zoom) > cap)
			return static_cast<int>(0.5f + cap * zoom);

		return value;
	}

	Geom::RectInt CropResampleRegions(const Geom::RectInt& region, const Geom::SizeInt& srcDims, const Geom::SizeInt& dstDims, float zoom) {
		int x1 = Util::Max(0, region.Left());
		int x2 = Util::Min(dstDims.Width + x1, region.Right());
		int y1 = Util::Max(0, region.Top());
		int y2 = Util::Min(dstDims.Height + y1, region.Bottom());

		x1 = crop_value(x1, zoom, srcDims.Width);
		x2 = crop_value(x2, zoom, srcDims.Width);
		y1 = crop_value(y1, zoom, srcDims.Height);
		y2 = crop_value(y2, zoom, srcDims.Height);

		return Geom::RectInt(Geom::PointInt(x1, y1), Geom::PointInt(x2, y2));
	}
}
