#include "StdAfx.h"

#include "illa/filter.h"

#include "share_filterbuffer.h"

using namespace Filter;
using Geom::RectInt;
using Geom::PointInt;
using Geom::SizeInt;
using namespace Tst;

SUITE(Filters)
{
	TEST_FIXTURE(ReferenceRotationFixture, Empty_Targets)
	{
		m_bufferDestination.filterBuffer.BufferData = 0;
		CHECK_THROW(Adjust::BrightnessContrast(m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, PointInt(10, 10), 100, 13), Err::InvalidParam);
		CHECK_THROW(Alpha::SolidColor(m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, PointInt(10, 10), Img::Color(200, 200, 200, 200)), Err::InvalidParam);
	}

	TEST(BrightnessContrastBufferPadding)
	{
		BufferInfo bi = ConstructBufferInfo(SizeInt(8, 8), 3, ClearDWord, DestinationPadding);
		Adjust::BrightnessContrast(bi.filterBuffer, bi.filterBuffer, RectInt(PointInt(0, 0), SizeInt(8, 8)), PointInt(0, 0), 5, 12);
		CHECK_EQUAL(CheckPaddingUntouched(bi, DestinationPadding), true);
	}

	TEST(GammaBufferPadding)
	{
		BufferInfo bi = ConstructBufferInfo(SizeInt(8, 8), 3, ClearDWord, DestinationPadding);
		Adjust::Gamma(bi.filterBuffer, bi.filterBuffer, RectInt(PointInt(0, 0), SizeInt(8, 8)), PointInt(0, 0), 5);
		CHECK_EQUAL(CheckPaddingUntouched(bi, DestinationPadding), true);
	}

	TEST(AlphaBufferPadding)
	{
		BufferInfo bi = ConstructBufferInfo(SizeInt(8, 8), 3, ClearDWord, DestinationPadding);
		Alpha::SolidColor(bi.filterBuffer, bi.filterBuffer, RectInt(PointInt(0, 0), SizeInt(8, 8)), PointInt(0, 0), Img::Color(3, 4, 5, 6));
		CHECK_EQUAL(CheckPaddingUntouched(bi, DestinationPadding), true);
	}

	struct ColorKeyFixtureNoPadding {
		ByteArray SourceBuffer;
		ByteArray DestBuffer;
		ByteArray ReferenceBuffer;

		FilterBuffer fbs, fbd, fbr;

		Img::Palette p;
		RectInt fullArea;

		ColorKeyFixtureNoPadding()
			:SourceBuffer(new uint8_t[16]),
			 DestBuffer(new uint8_t[64]),
			 ReferenceBuffer(new uint8_t[64])
		{
			fullArea = RectInt(PointInt(0, 0), SizeInt(4, 4));
			uint8_t c_SourceBuffer[] = {
				ClearDWord, ClearDWord, ClearDWord, ClearDWord,
				ClearDWord, 0, 1, ClearDWord,
				ClearDWord, 1, 0, ClearDWord,
				ClearDWord, ClearDWord, ClearDWord, ClearDWord
			};
			memcpy(SourceBuffer.get(), c_SourceBuffer, 4 * 4);
			p.Color(0, Img::Color::FromDWord(5));
			p.Color(0, Img::Color::FromDWord(1));
			fbs.Construct(SizeInt(4, 4), 1, SourceBuffer.get(), 4, p);

			uint32_t c_DestBuffer[] =
			{
				ClearDWord, ClearDWord, ClearDWord, ClearDWord,
				ClearDWord, 0, 3, ClearDWord,
				ClearDWord, 3, 0, ClearDWord,
				ClearDWord, ClearDWord, ClearDWord, ClearDWord
			};
			memcpy(DestBuffer.get(), c_DestBuffer, 4 * 4 * 4);
			fbd.Construct(SizeInt(4, 4), 4, DestBuffer.get(), 16);

			uint32_t c_Reference[] = 
			{
				ClearDWord, ClearDWord, ClearDWord, ClearDWord,
				ClearDWord, 0, 1, ClearDWord,
				ClearDWord, 1, 0, ClearDWord,
				ClearDWord, ClearDWord, ClearDWord, ClearDWord
			};
			memcpy(ReferenceBuffer.get(), c_Reference, 4 * 4 * 4);
			fbr.Construct(SizeInt(4, 4), 4, ReferenceBuffer.get(), 16);
		}
	};
}
