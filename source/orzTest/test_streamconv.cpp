#include "orz/streamconv.h"
#include "main.h"
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
		FILE* f = 0;
		CHECK(_wfopen_s(&f, (g_datapath+L"/data.raw").c_str(), L"rb") == 0);
		CHECK(f != 0);
		size_t read;
		do
		{
			uint8_t buf[ReadBuffer];
			read=fread(buf, 1, ReadBuffer, f);
			sc.AddBytes(buf, read);
		}
		while(read!=0);

		// Increase size to 16 bits and verify with the file contents
		sc.ChangeWordSize(16);

		CHECK(fseek(f, 0, SEEK_SET) == 0);

		do
		{
			uint16_t buf[ReadBuffer];
			read = fread(buf, 2, ReadBuffer, f);

			for(size_t i = 0; i < read; i++)
			{
				CHECK((uint16_t)sc.GetWord()==buf[i]);
			}
		}
		while(read != 0);

		if(f) fclose(f);
	}
}
