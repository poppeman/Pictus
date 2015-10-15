#include "orz/streamconv.h"
#include "main.h"
#include "orz/file_reader.h"
#include "orz/types.h"
#include <UnitTest++/UnitTest++.h>

SUITE(StreamConverter)
{
	TEST(Defaults)
	{
		Util::StreamConverter sc;

		// Should default to empty with wordsize 8
		CHECK(sc.CurrentWordSize() == 8);
		CHECK(sc.IsWordsLeft() == false);
	}

	TEST(ChangeWordSize)
	{
		Util::StreamConverter sc;
		sc.ChangeWordSize(16);
		CHECK(sc.CurrentWordSize() == 16);
	}

	TEST(IO)
	{
		const int ReadBuffer=1024;

		Util::StreamConverter sc;
		// Store an entire file in the stream converter
		auto f = std::make_shared<IO::FileReader>(g_datapath+"/data.raw");
		CHECK(f->Open());
		size_t read;
		do
		{
			uint8_t buf[ReadBuffer];
			read=f->Read(buf, 1, ReadBuffer);
			sc.AddBytes(buf, read);
		}
		while(read!=0);

		// Increase size to 16 bits and verify with the file contents
		sc.ChangeWordSize(16);

		f->Seek(0, IO::SeekMethod::Begin);

		do
		{
			uint16_t buf[ReadBuffer];
			read = f->Read(buf, 2, ReadBuffer);

			for(size_t i = 0; i < read; i++)
			{
				CHECK((uint16_t)sc.GetWord()==buf[i]);
			}
		}
		while(read != 0);
	}
}
