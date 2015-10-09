#include "color.h"
#include "color_conv.h"
#include <algorithm>

namespace Img {

	Color::Color(int cA, int cR, int cG, int cB) :A(cA), R(cR), G(cG), B(cB)
	{

	}

	Img::Color Color::FromDWord(uint32_t dw)
	{
		return Color(static_cast<uint8_t>(dw >> 24), static_cast<uint8_t>((dw & 0xff0000) >> 16), static_cast<uint8_t>((dw & 0xff00) >> 8), static_cast<uint8_t>(dw & 0xff));
	}

	Img::Color Color::FromInvertedCMYK(uint32_t c, uint32_t m, uint32_t y, uint32_t k)
	{
		return Img::Color(
			0xff,
			c * k / 255,
			m * k / 255,
			y * k / 255
		);
	}

	uint32_t Color::ToDWord() const
	{
		return (A << 24) + (R << 16) + (G << 8) + B;
	}

	std::basic_ostream<char>& operator<<(std::basic_ostream<char>& in, const Color& c)
	{
		in << "(" << (int)c.A << ", " << (int)c.R << ", " << (int)c.G << ", " << (int)c.B << ")";
		return in;
	}

	HLSTriplet RGB_HLS(uint8_t r, uint8_t g, uint8_t b)
	{
		HLSTriplet t;

		uint8_t min_val = std::min(r, std::min(g, b));
		uint8_t max_val = std::max(r, std::max(g, b));

		float sum = static_cast<float>(max_val + min_val);

		t.L = sum / 512.0f;

		if (min_val == max_val)
		{
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

	bool operator!=(const Color& lhs, const Color& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator==(const Color& lhs, const Color& rhs)
	{
		return ToARGBDWORD(lhs) == ToARGBDWORD(rhs);
	}

	uint8_t HueToRGB(float m1, float m2, float hue)
	{
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

	Img::Color HLS_RGB(float h, float l, float s)
	{
		if (s == 0.0f) {
			// Grey scale
			uint8_t v = static_cast<uint8_t>(l * 255.0f);
			return Img::Color(0, v, v, v);
		}
		else {
			// Magic numbers according to Microsoft :)
			float m2 = (l <= 0.5f) ? (l + l * s) : (l + s - l * s);
			float m1 = 2.0f * l - m2;

			return{ 0,
				HueToRGB(m1, m2, h + 120.0f),
				HueToRGB(m1, m2, h),
				HueToRGB(m1, m2, h - 120.0f) };
		}
	}

	Img::Color HLS_RGB(HLSTriplet& hls)
	{
		return HLS_RGB(hls.H, hls.L, hls.S);
	}

	Img::Color HLS(float h, float l, float s)
	{
		return Img::Color(HLS_RGB(h, l, s));
	}

	HLSTriplet::HLSTriplet()
	{

	}

	HLSTriplet::HLSTriplet(const float _H, const float _L, const float _S) :H(_H), L(_L), S(_S)
	{

	}

}
