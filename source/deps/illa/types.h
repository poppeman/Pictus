#ifndef ILLA_TYPES_H
#define ILLA_TYPES_H

#include "../orz/exception.h"

namespace Err {
	struct CodecError:public Exception {
		CodecError(const std::wstring& msg):Exception(TX("Codec error: ") + msg) {}
	};

	struct InvalidHeader:public CodecError {
		InvalidHeader(const std::wstring& msg):CodecError(TX("InvalidHeader: ") + msg) {}
	};
}

namespace Img {
	class Palette;
	class Image;
	class Surface;

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

std::wstring ToWString(const Img::Format& imgFormat);
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Img::Format& fmt);


#include "geom.h"

namespace Img {
	class Color {
	public:
		int A, R, G, B;

		Color(int cA, int cR, int cG, int cB)
			:A(cA), R(cR), G(cG), B(cB)
		{}

		static Color FromDWord(uint32_t dw) {
			return Color(static_cast<uint8_t>(dw >> 24), static_cast<uint8_t>((dw & 0xff0000) >> 16), static_cast<uint8_t>((dw & 0xff00) >> 8), static_cast<uint8_t>(dw&0xff));
		}

		static Color FromInvertedCMYK(uint32_t c, uint32_t m, uint32_t y, uint32_t k) {
			return {
				0xff,
				c * k / 255,
				m * k / 255,
				y * k / 255
			};
		}

		uint32_t ToDWord() const {
			return (A << 24) + (R << 16) + (G << 8) + B;
		}

		Color() {}
	};

	inline std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Img::Color& c) {
		in << "(" << (int)c.A << ", " << (int)c.R << ", " << (int)c.G << ", " << (int)c.B << ")";
		return in;
	}
}

#include "color_conv.h"

namespace Img {
	static const float HueCap = 360.0f;
	static const float LumCap = 1.0f;
	static const float SatCap = 1.0f;

	struct HLSTriplet {
		float H, L, S;
		HLSTriplet() {}
		HLSTriplet(const float _H, const float _L, const float _S)
			:H(_H), L(_L), S(_S)
		{}
	};

	HLSTriplet RGB_HLS(uint8_t r, uint8_t g, uint8_t b);
	uint8_t HueToRGB(float m1, float m2, float hue);

	Img::Color HLS_RGB(float h, float l, float s);

	static inline Img::Color HLS_RGB(HLSTriplet& hls) {
		return HLS_RGB(hls.H, hls.L, hls.S);
	}

	inline static Color HLS(float h, float l, float s) {
		return Img::Color(HLS_RGB(h, l, s));
	}

	inline bool operator==(const Color& lhs, const Color& rhs) {
		return ToARGBDWORD(lhs) == ToARGBDWORD(rhs);
	}

	inline bool operator!=(const Color& lhs, const Color& rhs) {
		return !(lhs == rhs);
	}

	class Palette {
	public:
		void Color(int index, const Color& col) {
			if (index > 255) {
				DO_THROW(Err::InvalidParam, TX("Color index out of bounds (> 255)"));
			}

			if (index < 0) {
				DO_THROW(Err::InvalidParam, TX("Color index out of bounds (< 0)"));
			}

			m_colors[index] = col;
		}

		const Img::Color& Color(int index) const {
			if (index > 255) {
				DO_THROW(Err::InvalidParam, TX("Color index out of bounds (> 255)"));
			}

			if (index < 0) {
				DO_THROW(Err::InvalidParam, TX("Color index out of bounds (< 0)"));
			}

			return m_colors[index];
		}

	private:
		Img::Color m_colors[256];
	};

	Img::Palette Grayscale(int minEntry = 0, int maxEntry = 255);

	bool operator==(const Palette& lhs, const Palette& rhs);
}


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

	struct FilterBuffer {
		Geom::SizeInt Dimensions;
		int PixelSize;
		uint8_t* BufferData;
		size_t Stride;
		Img::Palette Palette;

		void Construct(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_);
		void Construct(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal_);

		bool operator==(const FilterBuffer& rhs) const;
		bool operator!=(const FilterBuffer& rhs) const;

		FilterBuffer(const Geom::SizeInt& sz_ = Geom::SizeInt(0, 0), int ps_ = 0, uint8_t* data_ = 0, size_t stride_ = 0);
		FilterBuffer(const Geom::SizeInt& sz_, int ps_, uint8_t* data_, size_t stride_, const Img::Palette& pal);
	};

	Geom::SizeInt CalculateUnzoomedSize(_In_ Geom::SizeInt defaultDims, _In_ Filter::RotationAngle angle);
}


std::wstring ToWString(const Filter::RotationAngle& angle);
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Filter::RotationAngle& c);

namespace Img {
	struct Properties {
		int						Brightness;
		int						Contrast;
		int						Gamma;
		Filter::Mode		ResampleFilter;
		bool					RetainAlpha;
		Img::Color				BackgroundColor;
		float					Zoom;
		Filter::RotationAngle	Angle;

		Properties();
	};
}

#endif
