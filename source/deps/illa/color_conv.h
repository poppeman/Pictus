#ifndef COLOR_CONV_H
#define COLOR_CONV_H

#include "types.h"

namespace Img {
	inline uint32_t ToARGBDWORD(int A, int R, int G, int B)	{	return (A << 24) + (R << 16) + (G << 8) + B;	}
	inline uint32_t ToARGBDWORD(const Img::Color& color)		{	return (color.A << 24) + (color.R << 16) + (color.G << 8) + color.B;	}
	inline uint16_t ToRGBWord565(const Img::Color& color)		{	return ((color.R & 0xf8) << 11) | ((color.G & 0xfc) << 5) | (color.B & 0xf8);	}
	inline uint16_t ToARGBWord1555(const Img::Color& color)		{	return ((color.A & 0x80) << 15) | ((color.R & 0xf8) << 10) | ((color.G & 0xf8) << 5) | (color.B & 0xf8);	}
	inline uint8_t ChannelARGB8888Alpha(const uint32_t& val)		{	return static_cast<uint8_t>(val >> 24);	}
	inline uint8_t ChannelARGB8888Red(const uint32_t& val)		{	return static_cast<uint8_t>((val & 0xff0000) >> 16);	}
	inline uint8_t ChannelARGB8888Green(const uint32_t& val)		{	return static_cast<uint8_t>((val & 0xff00) >> 8);	}
	inline uint8_t ChannelARGB8888Blue(const uint32_t& val)		{	return static_cast<uint8_t>(val & 0xff);	}
	inline uint8_t ChannelARGB1555Alpha(const uint16_t& val)		{	return static_cast<uint8_t>(val >> 15);	}
	inline uint8_t ChannelARGB1555Red(const uint16_t& val)			{	return static_cast<uint8_t>((val & 0x7C00) >> 10);	}
	inline uint8_t ChannelARGB1555Green(const uint16_t& val)		{	return static_cast<uint8_t>((val & 0x03E0) >> 5);	}
	inline uint8_t ChannelARGB1555Blue(const uint16_t& val)		{	return static_cast<uint8_t>(val & 0x1f);	}
	inline uint8_t ChannelRGB565Red(const uint16_t& val)			{	return static_cast<uint8_t>((val & 0xF8C0) >> 11);	}
	inline uint8_t ChannelRGB565Green(const uint16_t& val)			{	return static_cast<uint8_t>((val & 0x07E0) >> 5);	}
	inline uint8_t ChannelRGB565Blue(const uint16_t& val)			{	return static_cast<uint8_t>(val & 0x1f);	}
	inline uint8_t Channel5To8(const uint8_t& val)				{	return (val << 3) | (val >> 2);	}
	inline uint8_t Channel6To8(const uint8_t& val)				{	return (val << 2) | (val >> 4);	}

	inline uint32_t ABGR_ARGB(const uint32_t& v) { return ToARGBDWORD(ChannelARGB8888Alpha(v), ChannelARGB8888Blue(v), ChannelARGB8888Green(v), ChannelARGB8888Red(v)); }

	inline uint32_t ConvertARGB1555ToARGB8888(const uint16_t& src) {
		return Img::ToARGBDWORD(
			(ChannelARGB1555Alpha(src) * 255),
			Channel5To8(ChannelARGB1555Red(src)),
			Channel5To8(ChannelARGB1555Green(src)),
			Channel5To8(ChannelARGB1555Blue(src)));
	}

	inline uint32_t ConvertXRGB1555ToARGB8888(const uint16_t& src) {
		return Img::ToARGBDWORD(
			0xff,
			Channel5To8(ChannelARGB1555Red(src)),
			Channel5To8(ChannelARGB1555Green(src)),
			Channel5To8(ChannelARGB1555Blue(src)));
	}

	inline uint32_t ConvertRGB565ToARGB8888(const uint16_t& src) {
		return Img::ToARGBDWORD(
			0xff,
			Channel5To8(ChannelRGB565Red(src)),
			Channel6To8(ChannelRGB565Green(src)),
			Channel5To8(ChannelRGB565Blue(src)));	}

	inline uint32_t MixARGB8888ARGB8888(const uint32_t& pix1, const uint32_t& pix2, uint8_t alpha) {
		int ia = alpha + ((alpha & 0x80) >> 7);
		uint32_t dstrb = pix1      & 0xFF00FF;
		uint32_t dstag = pix1 >> 8 & 0xFF00FF;

		uint32_t srcrb = pix2      & 0xFF00FF;
		uint32_t srcag = pix2 >> 8 & 0xFF00FF;

		uint32_t drb = srcrb - dstrb;
		uint32_t dag = srcag - dstag;

		drb *= (ia);
		dag *= (ia);
		drb >>= 8;
		dag >>= 8;

		const uint32_t rb  = (drb + dstrb)      & 0x00FF00FF;
		const uint32_t ag  = (dag + dstag) << 8 & 0xFF00FF00;

		return rb | ag;
	}
}

#endif
