#include <UnitTest++/UnitTest++.h>
#include "share_filterbuffer.h"

using namespace Filter;
using Geom::RectInt;
using Geom::PointInt;
using Geom::SizeInt;
using namespace Tst;

SUITE(Filters_RotateFixed)
{
	TEST_FIXTURE(ReferenceRotationFixture, QueryInplaceDefault)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotationAngle::RotateDefault, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplaceMirror)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotationAngle::FlipX, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplaceFlip)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotationAngle::FlipY, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplace90)
	{
		bool supportInplace = true;
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotationAngle::Rotate90, 0, &supportInplace);
		CHECK(supportInplace == false);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplace180)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotationAngle::Rotate180, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplace270)
	{
		bool supportInplace = true;
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotationAngle::Rotate270, 0, &supportInplace);
		CHECK(supportInplace == false);
	}
}
