#ifndef ILLA_COLOR_H
#define ILLA_COLOR_H

#include <cstdint>
#include <iostream>

namespace Img {
	class Color {
	public:
		int A = 0, R = 0, G = 0, B = 0;

		Color(int cA, int cR, int cG, int cB);

		static Color FromDWord(uint32_t dw);

		static Color FromInvertedCMYK(uint32_t c, uint32_t m, uint32_t y, uint32_t k);

		uint32_t ToDWord() const;

		Color() = default;
	};

	std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Img::Color& c);

	static const float HueCap = 360.0f;
	static const float LumCap = 1.0f;
	static const float SatCap = 1.0f;

	struct HLSTriplet {
		float H=0.0f, L=0.0f, S=0.0f;
		HLSTriplet();
		HLSTriplet(const float _H, const float _L, const float _S);
	};

	HLSTriplet RGB_HLS(uint8_t r, uint8_t g, uint8_t b);
	uint8_t HueToRGB(float m1, float m2, float hue);
	Img::Color HLS_RGB(float h, float l, float s);
	Img::Color HLS_RGB(HLSTriplet& hls);
	Color HLS(float h, float l, float s);
	bool operator==(const Color& lhs, const Color& rhs);
	bool operator!=(const Color& lhs, const Color& rhs);
}

#endif
