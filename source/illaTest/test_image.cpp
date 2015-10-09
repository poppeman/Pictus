#include <UnitTest++/UnitTest++.h>
#include "illa/image.h"

#include "main.h"

SUITE(ImageSuite)
{
/*	TEST(RepeatLoadHeader)
	{
		Img::Image image(g_datapath + L"/bw.tga");
		image.LoadHeader();
		CHECK_THROW(image.LoadHeader(), Err::CriticalError);
	}

	TEST(NoAllocate)
	{
		Img::Image image(g_datapath + L"/bw.tga");
		image.LoadHeader();
		CHECK_THROW(image.LoadImageData(), Err::CriticalError);
	}

	TEST(RepeatHeader)
	{
		Img::Image image(g_datapath + L"/bw.tga");
		image.LoadHeader();
		image.Deallocate();
		image.LoadHeader();
		CHECK(true);	// If the test reaches this point, it succeeded
	}

	TEST(RepeatLoadHeaderAbort)
	{
		Img::Image image(g_datapath + L"/bw.tga");
		image.LoadHeader();
		image.Abort();
		CHECK_THROW(image.LoadHeader(), Err::CriticalError);
	}*/
}
