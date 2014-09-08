#include "StdAfx.h"
#include "illa/types.h"

const int Threshold=2;
const int Step=20;

TEST(HLS_RGB_Conversion)
{
	// Convert to/from/to RGB/HLS/RGB and make sure the difference is small
	for(int r = 0; r < 256; r += Step)
	{
		for(int g = 0; g < 256; g += Step)
		{
			for(int b = 0; b < 256; b += Step)
			{
				uint8_t rb = static_cast<uint8_t>(r);
				uint8_t gb = static_cast<uint8_t>(g);
				uint8_t bb = static_cast<uint8_t>(b);

				Img::Color x(0, rb, gb, bb);
				Img::HLSTriplet y = Img::RGB_HLS(rb, gb, bb);
				Img::Color z(Img::HLS(y.H, y.L, y.S));

				CHECK(abs(x.R-z.R)<Threshold);
				CHECK(abs(x.G-z.G)<Threshold);
				CHECK(abs(x.B-z.B)<Threshold);
			}
		}
	}
}
