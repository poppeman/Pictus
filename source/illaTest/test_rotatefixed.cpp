#include <UnitTest++/UnitTest++.h>
#include "share_filterbuffer.h"

using namespace Filter;
using Geom::RectInt;
using Geom::PointInt;
using Geom::SizeInt;
using namespace Tst;

SUITE(Filters_RotateFixed)
{
	TEST_FIXTURE(ReferenceRotationFixture, Empty_Target)
	{
		m_bufferDestination.filterBuffer.BufferData = 0;
		CHECK_THROW(Transformation::RotateFixed(m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, m_positionTargetDefault, RotationAngle::RotateDefault), Err::InvalidParam);
	}

	TEST_FIXTURE(ReferenceRotationFixture, Empty_Source)
	{
		m_bufferSource.BufferData = 0;
		CHECK_THROW(Transformation::RotateFixed(m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, m_positionTargetDefault, RotationAngle::RotateDefault), Err::InvalidParam);
	}
	// To add: Invalid region, invalid top-left, cropping, non-matching strides, etc

	TEST_FIXTURE(ReferenceRotationFixture, InvalidAngle)
	{
		CHECK_THROW(Transformation::RotateFixed(m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, m_positionTargetDefault, Filter::RotationAngle::RotateUndefined), Err::InvalidParam);
		CHECK_THROW(Transformation::RotateFixed(m_bufferSource, m_bufferDestination.filterBuffer, m_regionDefault, m_positionTargetDefault, static_cast<Filter::RotationAngle>(42)), Err::InvalidParam);
	}

	TEST_FIXTURE(ReferenceRotationFixture, BasicMirror)
	{
		uint32_t cReferenceDestBufferMirror[] = 
		{
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding,
			DestinationPadding, DestinationPadding, 2, 1,
			DestinationPadding, DestinationPadding, 4, 3,
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding
		};

		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			{ Geom::PointInt{ 1, 1 }, Geom::SizeInt{ 2, 2 } },
			{ 2, 1 },
			Filter::RotationAngle::FlipX);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBufferMirror, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}

	TEST_FIXTURE(ReferenceRotationFixture, BasicFlip)
	{
		uint32_t cReferenceDestBuffer[] = 
		{
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding,
			DestinationPadding, DestinationPadding, 3, 4,
			DestinationPadding, DestinationPadding, 1, 2,
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding
		};

		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			{ Geom::PointInt(1, 1), Geom::SizeInt(2, 2) },
			{ 2, 1 },
			Filter::RotationAngle::FlipY);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBuffer, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}	

	TEST_FIXTURE(ReferenceRotationFixture, BasicRotate90)
	{
		uint32_t cReferenceDestBuffer[] = 
		{
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding,
			DestinationPadding, DestinationPadding, 3, 1,
			DestinationPadding, DestinationPadding, 4, 2,
			DestinationPadding, DestinationPadding, ClearDWord, ClearDWord
		};

		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			{ Geom::PointInt(1, 1), Geom::SizeInt(3, 2) },
			{ 2, 1 },
			Filter::RotationAngle::Rotate90);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBuffer, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}

	TEST_FIXTURE(ReferenceRotationFixture, BasicRotate180)
	{
		uint32_t cReferenceDestBuffer[] = 
		{
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding,
			DestinationPadding, DestinationPadding, 4, 3,
			DestinationPadding, DestinationPadding, 2, 1,
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding
		};

		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			{ Geom::PointInt(1, 1), Geom::SizeInt(2, 2) },
			{ 2, 1 },
			Filter::RotationAngle::Rotate180);
		CHECK_EQUAL(BuffersEqual(cReferenceDestBuffer, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}

	TEST_FIXTURE(ReferenceRotationFixture, BasicRotate270)
	{
		uint32_t cReferenceDestBuffer[] = 
		{
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding,
			DestinationPadding, DestinationPadding, 2, 4,
			DestinationPadding, DestinationPadding, 1, 3,
			DestinationPadding, DestinationPadding, DestinationPadding, DestinationPadding
		};

		Transformation::RotateFixed(
			m_bufferSource,
			m_bufferDestination.filterBuffer,
			{ Geom::PointInt(1, 1), Geom::SizeInt(2, 2) },
			{ 2, 1 },
			Filter::RotationAngle::Rotate270);
		CHECK_EQUAL(true == BuffersEqual(cReferenceDestBuffer, (uint32_t*)m_bufferDestination.filterBuffer.BufferData, 4 * 4), true);
	}
}
