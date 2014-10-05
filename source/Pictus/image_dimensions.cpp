#include "StdAfx.h"
#include "image_dimensions.h"

namespace App {
	using namespace Geom;

	Geom::SizeInt ImageDimensions::ImageSize() const {
		return m_image;
	}

	Geom::SizeInt ImageDimensions::SurfaceSize() const {
		return m_surface;
	}

	Geom::SizeInt ImageDimensions::EffectiveSize() const {
		return m_image;
	}

	Geom::RectInt ImageDimensions::TranslatedSurfaceSourceRect(const Geom::RectInt& destArea) const {
		if (m_angle == Filter::RotationAngle::Rotate90) {
			return{ PointInt{ destArea.Top(), m_image.Height - destArea.Left() - destArea.Width() }, destArea.Dimensions().Flipped() };
		}
		if (m_angle == Filter::RotationAngle::Rotate180) {
			return{ PointInt{ m_image.Width - destArea.Left() - destArea.Width(), m_image.Height - destArea.Top() - destArea.Height() }, destArea.Dimensions() };
		}
		if (m_angle == Filter::RotationAngle::Rotate270) {
			return{ PointInt{ m_image.Width - destArea.Top() - destArea.Height(), destArea.Left() }, destArea.Dimensions().Flipped() };
		}
		if (m_angle == Filter::RotationAngle::FlipX) {
			return{ PointInt{ m_image.Width - destArea.Left() - destArea.Width(), destArea.Top() }, destArea.Dimensions() };
		}
		if (m_angle == Filter::RotationAngle::FlipY) {
			return{ PointInt{destArea.Left(), m_image.Height - destArea.Top() - destArea.Height() }, destArea.Dimensions() };
		}

		if (m_angle == Filter::RotationAngle::RotateDefault) {
			return destArea;
		}

		DO_THROW(Err::InvalidCall, TX("Invalid angle."));
	}

	void ImageDimensions::Angle(Filter::RotationAngle angle) {
		m_angle = angle;
	}

	Filter::RotationAngle ImageDimensions::Angle() const {
		return m_angle;
	}

	void ImageDimensions::SetImageSize(const Geom::SizeInt sz) {
		m_image = sz;
	}

	void ImageDimensions::SetSurfaceSize(const Geom::SizeInt sz) {
		m_surface = sz;
	}

	ImageDimensions::ImageDimensions() :m_angle{ Filter::RotationAngle::RotateDefault } {}
}
