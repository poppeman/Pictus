#ifndef IMAGE_DIMENSIONS
#define IMAGE_DIMENSIONS

#include "illa/geom.h"
#include "illa/types.h"

namespace App {
	class ImageDimensions {
	public:
		Geom::SizeInt ImageSize() const;
		Geom::SizeInt SurfaceSize() const;
		Geom::SizeInt EffectiveSize() const;

		Geom::RectInt TranslatedSurfaceSourceRect(const Geom::RectInt& destArea) const;

		void Angle(Filter::RotationAngle angle);
		Filter::RotationAngle Angle() const;

		void SetImageSize(const Geom::SizeInt sz);
		void SetSurfaceSize(const Geom::SizeInt sz);

		ImageDimensions();

	private:
		Geom::SizeInt m_image;
		Geom::SizeInt m_surface;
		Filter::RotationAngle m_angle;
	};
}

#endif
