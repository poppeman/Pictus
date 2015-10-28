#include <UnitTest++/UnitTest++.h>
#include "illa/filter_int.h"


namespace Img
{
	Img::Color operator+(const Img::Color& lhs, const Img::Color& rhs)
	{
		return Img::Color(
			lhs.A+rhs.A,
			lhs.R+rhs.R,
			lhs.G+rhs.G,
			lhs.B+rhs.B);
	}

	Img::Color operator-(const Img::Color& lhs, const Img::Color& rhs)
	{
		return Img::Color(
			lhs.A-rhs.A,
			lhs.R-rhs.R,
			lhs.G-rhs.G,
			lhs.B-rhs.B);
	}

	bool operator >=(const Img::Color& lhs, const Img::Color& rhs)
	{
		return 
			(lhs.A >= rhs.A) &&
			(lhs.R >= rhs.R) &&
			(lhs.G >= rhs.G) &&
			(lhs.B >= rhs.B);
	}


	bool operator <=(const Img::Color& lhs, const Img::Color& rhs)
	{
		return 
			(lhs.A <= rhs.A) &&
			(lhs.R <= rhs.R) &&
			(lhs.G <= rhs.G) &&
			(lhs.B <= rhs.B);
	}
}

SUITE(ToRgb8888)
{
	TEST(From565To8888)
	{
		for(int r = 0; r < 32; ++r)
			for(int g = 0; g < 64; ++g)
				for(int b = 0; b < 32; ++b)
				{
					Img::Color ref(0xff, 
						static_cast<uint8_t>((float)r * (255.0f / 31.0f)),
						static_cast<uint8_t>((float)g * (255.0f / 63.0f)),
						static_cast<uint8_t>((float)b * (255.0f / 31.0f)));
					uint16_t orig = static_cast<uint16_t>((r << 11) + (g << 5) + b);
					Img::Color result(Img::Color::FromDWord(Img::ConvertRGB565ToARGB8888(orig)));

					CHECK_CLOSE(ref, result, Img::Color(1, 1, 1, 1));
				}
	}

	TEST(From1555To8888)
	{
		for(int a = 0; a < 2; ++a)
			for(int r = 0; r < 32; ++r)
				for(int g = 0; g < 32; ++g)
					for(int b = 0; b < 32; ++b)
					{
						Img::Color ref(static_cast<uint8_t>(a * 255), 
							static_cast<uint8_t>((float)r * (255.0f / 31.0f)),
							static_cast<uint8_t>((float)g * (255.0f / 31.0f)),
							static_cast<uint8_t>((float)b * (255.0f / 31.0f)));
						uint16_t orig = static_cast<uint16_t>((a << 15) + (r << 10) + (g << 5) + b);
						Img::Color result(Img::Color::FromDWord(Img::ConvertARGB1555ToARGB8888(orig)));

						CHECK_CLOSE(ref, result, Img::Color(1, 1, 1, 1));
					}
	}
}
 