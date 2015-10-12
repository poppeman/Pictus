#ifndef FILTER_H
#define FILTER_H

#include "types.h"

namespace Filter {
	namespace Scale {
		/**
		* Use filter indicated by the mode parameter.
		* Filtering can be based on any surface format but the destination must always be a 32-bit
		* ARGB or XRGB buffer (just disregard the alpha component if it isn't applicable).
		* If the region doesn't fit within the source- or destination buffers the region will be cropped.
		*
		* @param mode				Filtering mode to use.
		* @param source				A FilterBuffer object filled with the parameters for the source buffer.
		* @param dest				A FilterBuffer object filled with the parameters for the destination buffer.
		* @param region				Denotes the top-left corner of the DESTINATION area and the size of the DESTINATION area.
		* @param format				Buffer format for the source buffer.
		* @param zoom				Zoom factor.
		* @return					true if the operation was successful, otherwise false.
		*/
		void AutoFilter(const Filter::Mode mode, const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom);
	}

	namespace Transformation {
		bool IsInplace(RotationAngle angle);
		void QuerySizeRotateFixed(const FilterBuffer& source, RotationAngle angle, Geom::SizeInt* requiredSize, bool* supportInPlace);
		void RotateFixed(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, const Geom::PointInt& destTopLeft, RotationAngle angle);
	}

	namespace Adjust {
		void BrightnessContrast(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, const int newBright, const int newContrast);
		void Gamma(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, const int gamma);
	}

	namespace Alpha {
		void SolidColor(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, const Img::Color& bgrColor);
		void IndexedColorKey(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& sourceRegion, const Geom::PointInt& destPos, Img::Format sourceFormat, uint8_t colorKeyIndex);

		// Precondition: Buffer must be ARGB8888
		void PremultiplyAlphaBuffer(const FilterBuffer& source);
		// Precondition: Buffer must be ARGB8888 or XRGB8888
		void SetAlpha(const FilterBuffer& source, int alpha);
	}
}

#endif
