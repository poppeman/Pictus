#include <UnitTest++/UnitTest++.h>
#include "illa/filter.h"
#include "illa/codec.h"

#include "share_filterbuffer.h"
#include "illa/filter_int.h"

using namespace Filter;
using Geom::RectInt;
using Geom::PointInt;
using Geom::SizeInt;
using namespace Tst;

class StubCodec :public Img::AbstractCodec
{
private:
	Img::ImageComposer::Ptr RequestImageComposer() override
	{
		return nullptr;
	}

	bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo & info) override
	{
		return false;
	}

	LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override
	{
		return LoadStatus::Finished;
	}
};

SUITE(AbstractCodecTests)
{
	TEST(TestNullReader)
	{
		StubCodec c;
		CHECK(!c.LoadHeader(nullptr));
	}

	TEST(TestSkipHeader)
	{
		StubCodec c;
		CHECK_EQUAL(Img::AbstractCodec::AllocationStatus::Failed, c.Allocate());
	}
}
