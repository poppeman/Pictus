#include "StdAfx.h"

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
		Transformation::QuerySizeRotateFixed(m_bufferSource, RotateDefault, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplaceMirror)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, FlipX, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplaceFlip)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, FlipY, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplace90)
	{
		bool supportInplace = true;
		Transformation::QuerySizeRotateFixed(m_bufferSource, Rotate90, 0, &supportInplace);
		CHECK(supportInplace == false);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplace180)
	{
		bool supportInplace = false;
		Transformation::QuerySizeRotateFixed(m_bufferSource, Rotate180, 0, &supportInplace);
		CHECK(supportInplace);
	}

	TEST_FIXTURE(ReferenceRotationFixture, QueryInplace270)
	{
		bool supportInplace = true;
		Transformation::QuerySizeRotateFixed(m_bufferSource, Rotate270, 0, &supportInplace);
		CHECK(supportInplace == false);
	}
}
