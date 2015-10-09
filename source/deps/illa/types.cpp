#include "types.h"
#include "config.h"

namespace Filter {
	void FilterBuffer::Construct( const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_) {
		Dimensions = sz_;
		PixelSize = ps_;
		BufferData = data_;
		Stride = stride_;
	}

	void FilterBuffer::Construct( const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal_ ) {
		Dimensions = sz_;
		PixelSize = ps_;
		BufferData = data_;
		Stride = stride_;
		Palette = pal_;
	}

	bool FilterBuffer::operator==( const FilterBuffer& rhs ) const {
		return (Dimensions == rhs.Dimensions) && (PixelSize == rhs.PixelSize) && (BufferData == rhs.BufferData) && (Stride == rhs.Stride) && (Palette == rhs.Palette);
	}

	bool FilterBuffer::operator!=( const FilterBuffer& rhs ) const {
		return !(*this == rhs);
	}

	FilterBuffer::FilterBuffer( const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_) {
		Construct(sz_, ps_, data_, stride_);
	}

	FilterBuffer::FilterBuffer( const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal_  ) {
		Construct(sz_, ps_, data_, stride_, pal_);
	}

	_Use_decl_annotations_ Geom::SizeInt CalculateUnzoomedSize(Geom::SizeInt defaultDims, RotationAngle angle) {
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

	Img::Palette Grayscale(int minEntry, int maxEntry) {
		Img::Palette pal;

		int entryBoundary = abs(maxEntry - minEntry);

		if (minEntry < maxEntry) {
			for(int i = 0; i <= entryBoundary; i++) {
				uint8_t intensity = static_cast<uint8_t>(i * 255 / entryBoundary);
				pal.Color(i + minEntry, Img::Color(0xff, intensity, intensity, intensity));
			}
		}
		else {
			for(int i = 0; i <= entryBoundary; i++) {
				uint8_t intensity = static_cast<uint8_t>(255 - i * 255 / entryBoundary);
				pal.Color(i + maxEntry, Img::Color(0xff, intensity, intensity, intensity));
			}
		}
		return pal;
	}

	bool operator==(const Palette& lhs, const Palette& rhs) {
		for (int i = 0; i < 256; ++i) {
			if (lhs.Color(i) != rhs.Color(i)) {
				return false;
			}
		}
		return true;
	}

	HLSTriplet RGB_HLS(uint8_t r, uint8_t g, uint8_t b) {
		HLSTriplet t;

		uint8_t min_val = std::min(r, std::min(g, b));
		uint8_t max_val = std::max(r, std::max(g, b));

		float sum = static_cast<float>(max_val + min_val);

		t.L=sum/512.0f;

		if (min_val==max_val) {
			t.S = 0.0f;
			t.H = 0.0f;
		}
		else {
			float diff = static_cast<float>(max_val - min_val);

			// Calculate normalized components
			float r_norm = (max_val - r) / diff;
			float g_norm = (max_val - g) / diff;
			float b_norm = (max_val - b) / diff;

			t.S = (t.L <= 0.5f) ? (diff / sum) : (diff / (512.0f - sum));

			if (r == max_val) {
				t.H = 60.0f * (6.0f + b_norm - g_norm);
			}
			else if (g == max_val) {
				t.H = 60.0f * (2.0f + r_norm - b_norm);
			}
			else if (b == max_val) {
				t.H = 60.0f * (4.0f + g_norm - r_norm);
			}

			if (t.H > 360.0f) {
				t.H -= 360.0f;
			}
		}
		return t;
	}

	uint8_t HueToRGB(float m1, float m2, float hue) {
		// Make sure the hue is in the correct range (one full circle)
		if (hue > 360.0f) {
			hue -= 360.0f;
		}
		else if (hue < 0.0f) {
			hue += 360.0f;
		}

		if (hue < 60.0f) {
			m1 = m1 + (m2 - m1) * hue / 60.0f;
		}
		else if (hue < 180.0f) {
			m1 = m2;
		}
		else if (hue < 240.0f) {
			m1 = m1 + (m2 - m1) * (240.0f - hue) / 60.0f;
		}
		return static_cast<uint8_t>(m1 * 255.0f);
	}

	Img::Color HLS_RGB(float h, float l, float s) {
		if (s == 0.0f) {
			// Grey scale
			uint8_t v = static_cast<uint8_t>(l * 255.0f);
			return Img::Color(0, v, v, v);
		}
		else {
			// Magic numbers according to Microsoft :)
			float m2 = (l <= 0.5f)?(l + l * s):(l + s - l * s);
			float m1 = 2.0f * l - m2;

			return{ 0,
				HueToRGB(m1, m2, h + 120.0f),
				HueToRGB(m1, m2, h),
				HueToRGB(m1, m2, h - 120.0f) };
		}
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
