#ifndef ILLA_TYPES_H
#define ILLA_TYPES_H

#include "orz/exception.h"
#include <cstdint>

namespace Err {
	struct CodecError:public Exception {
		CodecError(const std::string& msg):Exception("Codec error: " + msg) {}
	};

	struct InvalidHeader:public CodecError {
		InvalidHeader(const std::string& msg):CodecError("InvalidHeader: " + msg) {}
	};

	struct AllocationError :public Exception {
		AllocationError(const std::string& msg) :Exception("Allocation failed: " + msg) {}
	};
}

namespace Img {
	enum class Format {
		Undefined,
		ARGB8888,
		XRGB8888,
		ARGB1555,
		XRGB1555,
		RGB565,
		Index8,
		Num
	};

	uint8_t EstimatePixelSize(Format sf);
	bool HasAlpha(Format sf);
}

std::string ToAString(const Img::Format& imgFormat);
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Img::Format& fmt);

#include "color.h"
#include "color_conv.h"
#include "filter_buffer.h"
#include "geom.h"
#include "palette.h"


namespace Filter {
	enum class Mode {
		Undefined = 255,
		DirectCopy = 0,
		NearestNeighbor,
		Bilinear,
		Lanczos3,
		Num
	};

	enum class RotationAngle {
		RotateDefault,
		FlipX,
		FlipY,
		Rotate90,
		Rotate180,
		Rotate270,
		RotateUndefined
	};
	
	Geom::SizeInt CalculateUnzoomedSize(Geom::SizeInt defaultDims, Filter::RotationAngle angle);
}


std::string ToAString(const Filter::RotationAngle& angle);
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Filter::RotationAngle& c);

namespace Img {
	struct Properties {
		int						Brightness;
		int						Contrast;
		int						Gamma;
		Filter::Mode			ResampleFilter;
		bool					RetainAlpha;
		Img::Color				BackgroundColor;
		float					Zoom;
		Filter::RotationAngle	RequestedAngle;
		Filter::RotationAngle	MetaAngle;

		Filter::RotationAngle FinalAngle() const;

		Properties();
	};
}

#endif
