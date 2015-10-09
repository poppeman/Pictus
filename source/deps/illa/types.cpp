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

	bool HasAlpha( Format sf ) {
		switch(sf) {
		case Format::ARGB1555:
		case Format::ARGB8888:
			return true;
		case Format::Index8:
			return true;
		}
		return false;
	}





	Properties::Properties() :
		Brightness{ 0 },
		Contrast{ ContrastStep },
		Gamma{ 10 },
		ResampleFilter{ Filter::Mode::NearestNeighbor },
		RetainAlpha{ false },
		BackgroundColor{ 0xff, 0xff, 0xff, 0xff },
		Zoom{ 1.0f },
		Angle{ Filter::RotationAngle::RotateDefault } {}
}

std::wstring ToWString(const Img::Format& imgFormat) {
	std::stringstream ss;
	ss << imgFormat;
	return ToWString(ss.str().c_str());
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

std::wstring ToWString(const Filter::RotationAngle& angle) {
	std::stringstream ss;
	ss << angle;
	return ToWString(ss.str().c_str());
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
