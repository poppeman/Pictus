#include "types.h"
#include "config.h"

#include "orz/types.h"

#include <sstream>

namespace Filter {
	Geom::SizeInt CalculateUnzoomedSize(Geom::SizeInt defaultDims, RotationAngle angle) {
		if (angle == Filter::RotationAngle::Rotate90 || angle == Filter::RotationAngle::Rotate270) {
			return defaultDims.Flipped();
		}
		return defaultDims;
	}
}


namespace Img {
	uint8_t EstimatePixelSize(Format sf) {
		switch (sf) {
		case Format::ARGB8888:
		case Format::XRGB8888:
			return 4;
		case Format::ARGB1555:
		case Format::XRGB1555:
		case Format::RGB565:
			return 2;
		case Format::Index8:
			return 1;
		default:
			return 0;
		}
	}

	bool HasAlpha(Format sf) {
		switch (sf) {
		case Format::ARGB1555:
		case Format::ARGB8888:
			return true;
		case Format::Index8:
			return true;
		}
		return false;
	}

	Filter::RotationAngle RotateRight(Filter::RotationAngle in)
	{
		switch (in)
		{
		case Filter::RotationAngle::RotateDefault:
			return Filter::RotationAngle::Rotate90;
		case Filter::RotationAngle::FlipX:
			return in; // TODO: Implement
		case Filter::RotationAngle::FlipY:
			return in;
		case Filter::RotationAngle::Rotate90:
			return Filter::RotationAngle::Rotate180;
		case Filter::RotationAngle::Rotate180:
			return Filter::RotationAngle::Rotate270;
		case Filter::RotationAngle::Rotate270:
			return Filter::RotationAngle::RotateDefault;
		case Filter::RotationAngle::RotateUndefined:
		default:
			DO_THROW(Err::InvalidParam, "(Img::RotateRight) Input angle not valid");
			break;
		}
	}

	Filter::RotationAngle Properties::FinalAngle() const
	{
		using Filter::RotationAngle;

		if (MetaAngle == RotationAngle::RotateDefault) return RequestedAngle;
		if (MetaAngle == RotationAngle::Rotate90) {
			return RotateRight(RequestedAngle);
		}
		if (MetaAngle == RotationAngle::Rotate180) {
			return RotateRight(RotateRight(RequestedAngle));
		}
		if(MetaAngle == RotationAngle::Rotate270) {
			return RotateRight(RotateRight(RotateRight(RequestedAngle)));
		}
		DO_THROW(Err::InvalidCall, "(Properties::FinalAngle) Unhandled angle state.");
	}

	Properties::Properties() :
		Brightness{ 0 },
		Contrast{ ContrastStep },
		Gamma{ 10 },
		ResampleFilter{ Filter::Mode::NearestNeighbor },
		RetainAlpha{ false },
		BackgroundColor{ 0xff, 0xff, 0xff, 0xff },
		Zoom{ 1.0f },
		RequestedAngle{ Filter::RotationAngle::RotateDefault },
		MetaAngle{ Filter::RotationAngle::RotateDefault }
	{}
}

std::string ToAString(const Img::Format& imgFormat) {
	std::stringstream ss;
	ss << imgFormat;
	return ss.str();
}

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Filter::RotationAngle& c) {
	switch (c) {
	case Filter::RotationAngle::FlipX:
		in << "FlipX";
		break;
	case Filter::RotationAngle::FlipY:
		in << "FlipY";
		break;
	case Filter::RotationAngle::Rotate90:
		in << "Rotate90";
		break;
	case Filter::RotationAngle::Rotate180:
		in << "Rotate180";
		break;
	case Filter::RotationAngle::Rotate270:
		in << "Rotate270";
		break;
	case Filter::RotationAngle::RotateDefault:
		in << "RotateDefault";
		break;
	case Filter::RotationAngle::RotateUndefined:
		in << "RotateUndefined";
		break;
	default:
		in << "Unknown(" << static_cast<int>(c) << ")";
		break;
	}
	return in;
}

std::string ToAString(const Filter::RotationAngle& angle) {
	std::stringstream ss;
	ss << angle;
	return ss.str();
}

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Img::Format& fmt) {
	switch (fmt) {
	case Img::Format::Undefined:
		in << "Undefined";
		break;
	case Img::Format::ARGB8888:
		in << "ARGB8888";
		break;
	case Img::Format::XRGB8888:
		in << "XRGB8888";
		break;
	case Img::Format::ARGB1555:
		in << "ARGB1555";
		break;
	case Img::Format::XRGB1555:
		in << "XRGB1555";
		break;
	case Img::Format::RGB565:
		in << "RGB565";
		break;
	case Img::Format::Index8:
		in << "Indexed8";
		break;
	default:
		in << "Unknown(" << static_cast<int>(fmt) << ")";
		break;
	}
	return in;
}
