#include "StdAfx.h"

#include "share_filterbuffer.h"
#include "illa/render.h"

using namespace Filter;
using namespace Geom;
using namespace Tst;

SUITE(Render)
{
	TEST(RetainAlphaDirectCopyARGB8888) {
		BufferInfo src = ConstructBufferInfo(SizeInt(16, 16), 0);
		BufferInfo dst = ConstructBufferInfo(SizeInt(16, 16), 0xffffffff);
		RectInt reg(PointInt(0, 0), SizeInt(16, 16));
		Img::Properties props;
		props.RetainAlpha = true;

		Img::RenderToBuffer(dst.filterBuffer, src.filterBuffer, Img::Format::ARGB8888, reg, props);

		for(int i = 0; i < 16 * 16 * 4; ++i)
			CHECK_EQUAL(0, dst.contents.get()[i]);

		src = ConstructBufferInfo(SizeInt(16, 16), 0xffffffff);
		Img::RenderToBuffer(dst.filterBuffer, src.filterBuffer, Img::Format::ARGB8888, reg, props);

		for(int i = 0; i < 16 * 16 * 4; ++i)
			CHECK_EQUAL(0xff, (uint8_t)(dst.contents.get()[i]));
	}
}
