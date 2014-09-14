#include "StdAfx.h"

#include "illa/filter.h"

#include "share_filterbuffer.h"
#include "illa/filter_int.h"

using namespace Filter;
using Geom::RectInt;
using Geom::PointInt;
using Geom::SizeInt;
using namespace Tst;

SUITE(Filters_Resample)
{
	TEST(RegionCropping)
	{
		RectInt result = CropResampleRegions(
			RectInt(PointInt(0, 362), SizeInt(500, 253)),
			SizeInt(500, 250),
			SizeInt(500, 253),
			1.0f);

		CHECK(result.Height() == 0);
	}

	TEST_FIXTURE(ReferenceRotationFixture, Empty_Targets)
	{
		// Filter to empty targets. All of these should fail.
		m_bufferDestination.filterBuffer.BufferData = 0;

		for (auto i = 0; i < static_cast<int>(Mode::Num); i++) {
			CHECK_THROW(Scale::AutoFilter(Filter::Mode(i), m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 1.0), Err::InvalidParam);
		}
	}

	TEST_FIXTURE(ReferenceRotationFixture, Empty_Sources)
	{
		// Filter from empty sources. All of these should fail.
		m_bufferSource.BufferData = 0;

		for (int i = 0; i < static_cast<int>(Mode::Num); i++)
			CHECK_THROW(Scale::AutoFilter(Filter::Mode(i), m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 1.0), Err::InvalidParam);
	}

	TEST_FIXTURE(ReferenceRotationFixture, Invalid_Filters)
	{
		// Use invalid filters
		CHECK_THROW(Scale::AutoFilter(Mode::Undefined, m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 1.3f), Err::InvalidParam);
		CHECK_THROW(Scale::AutoFilter(Mode::Num, m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 1.3f), Err::InvalidParam);

		// Use invalid formats
		for (int i = 0; i < static_cast<int>(Mode::Num); i++)
		{
			auto m = Filter::Mode(i);
			CHECK_THROW(Scale::AutoFilter(m, m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::Num, (m == Mode::DirectCopy ? 1.0f : 1.3f)), Err::InvalidParam);
			CHECK_THROW(Scale::AutoFilter(m, m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format(static_cast<int>(Img::Format::Num) + 3), (m == Mode::DirectCopy ? 1.0f : 1.3f)), Err::InvalidParam);
		}
	}

	TEST_FIXTURE(PaddedBuffersFixture, DirectCopy_NonMatchingStride)
	{
		Scale::AutoFilter(Mode::DirectCopy, m_bufferSource.filterBuffer, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 1.0f);
		CHECK_EQUAL(CheckPaddingUntouched(m_bufferDestination, DestinationPadding), true);
	}

	TEST_FIXTURE(PaddedBuffersFixture, Nearest_NonMatchingStride)
	{
		Scale::AutoFilter(Mode::NearestNeighbor, m_bufferSource.filterBuffer, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 2.0f);
		CHECK_EQUAL(CheckPaddingUntouched(m_bufferDestination, DestinationPadding), true);
	}

	TEST_FIXTURE(PaddedBuffersFixture, Bilinear_NonMatchingStride)
	{
		Scale::AutoFilter(Mode::Bilinear, m_bufferSource.filterBuffer, m_bufferDestination.filterBuffer, m_regionDefault, Img::Format::ARGB8888, 2.0f);
		CHECK_EQUAL(CheckPaddingUntouched(m_bufferDestination, DestinationPadding), true);
	}
}
