#include <UnitTest++/UnitTest++.h>
#include "share_filterbuffer.h"

using namespace Filter;
using Geom::RectInt;
using Geom::PointInt;
using Geom::SizeInt;
using namespace Tst;


SUITE(Filters_RotateFixed)
{
	const Geom::RectInt c_RegionInplace(Geom::RectInt(Geom::PointInt(1, 1), Geom::SizeInt(2, 2)));

	TEST_FIXTURE(ReferenceRotationFixture, BasicMirrorInplace)
	{
		uint32_t cReferenceDestBufferMirror[] = 
		{
			ClearDWord, ClearDWord, ClearDWord, ClearDWord,
			ClearDWord, 2, 1, ClearDWord,
			ClearDWord, 4, 3, ClearDWord,
			ClearDWord, ClearDWord, ClearDWord, ClearDWord
		};
		Inplace();
		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			c_RegionInplace,
			{ 1, 1 },
			Filter::RotationAngle::FlipX);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBufferMirror, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}


	TEST_FIXTURE(ReferenceRotationFixture, BasicFlipInplace)
	{
		uint32_t cReferenceDestBuffer[] = 
		{
			ClearDWord, ClearDWord, ClearDWord, ClearDWord,
			ClearDWord, 3, 4, ClearDWord,
			ClearDWord, 1, 2, ClearDWord,
			ClearDWord, ClearDWord, ClearDWord, ClearDWord
		};

		Inplace();
		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			c_RegionInplace,
			{ 1, 1 },
			Filter::RotationAngle::FlipY);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBuffer, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}

	TEST_FIXTURE(ReferenceRotationFixture, BasicRotate90Inplace)
	{
		CHECK_THROW(Transformation::RotateFixed(
			m_bufferDestination.filterBuffer,
			m_bufferDestination.filterBuffer,
			c_RegionInplace,
			{ 1, 1 },
			Filter::RotationAngle::Rotate90), Err::InvalidParam);
	}


	TEST_FIXTURE(ReferenceRotationFixture, BasicRotate180Inplace)
	{
		uint32_t cReferenceDestBuffer[] = 
		{
			ClearDWord, ClearDWord, ClearDWord, ClearDWord,
			ClearDWord, 4, 3, ClearDWord,
			ClearDWord, 2, 1, ClearDWord,
			ClearDWord, ClearDWord, ClearDWord, ClearDWord
		};

		Inplace();
		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			c_RegionInplace,
			{ 1, 1 },
			Filter::RotationAngle::Rotate180);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBuffer, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}

	TEST_FIXTURE(ReferenceRotationFixture, BasicRotate270Inplace)
	{
		CHECK_THROW(Transformation::RotateFixed(
			m_bufferDestination.filterBuffer,
			m_bufferDestination.filterBuffer,
			c_RegionInplace,
			{ 1, 1 },
			Filter::RotationAngle::Rotate270), Err::InvalidParam);
	}

}
