#include "orz/file_reader.h"
#include "orz/fileops.h"
#include "main.h"
#include <UnitTest++/UnitTest++.h>

enum
{
	SeekDelta = 100
};

#pragma warning(disable: 6031)

SUITE(IO)
{
	class ReaderTestFixture
	{
	public:
		ReaderTestFixture():file(g_datapath + "/data.raw")
		{
			file.Open();
		}

		~ReaderTestFixture() {}

		IO::FileReader file;
	};

	class ReaderAsciiFixture
	{
	public:
		ReaderAsciiFixture():
			content("This is a simple text file."), file(g_datapath + "/ascii.txt")
		{
			file.Open();
		}

		~ReaderAsciiFixture() {}

		IO::FileReader file;
		std::string content;
	};

	TEST(FileReader_EmptyState)
	{
		IO::FileReader f(g_datapath + "/ascii.txt");
		CHECK(f.IsOpen() == false);	// Shouldn't be open
		CHECK_THROW(f.Position(), Err::FileNotOpen);	// Shouldn't be able to check position
		CHECK_THROW(f.Size(), Err::FileNotOpen); // Shouldn't be able to get file size

		uint8_t buf;
		CHECK_THROW(f.Read(&buf, 1, 1), Err::FileNotOpen);	// Read should fail
	}

	TEST(FileReader_Open)
	{
		IO::FileReader f(g_datapath + "/data.raw");
		CHECK(f.Open());
	}

	TEST_FIXTURE(ReaderTestFixture, FileReader_OpenedState)
	{
		CHECK(file.IsOpen() == true);
		CHECK(file.Position() == 0);
	}

	TEST_FIXTURE(ReaderTestFixture, FileReader_SeekBegin)
	{
		file.Seek(SeekDelta, IO::SeekMethod::Begin);
		CHECK(file.Position() == SeekDelta);
	}

	TEST_FIXTURE(ReaderTestFixture, FileReader_SeekCurrent)
	{
		// Should just move forward SeekDelta amount of bytes
		file.Seek(SeekDelta, IO::SeekMethod::Current);
		CHECK(file.Position() == SeekDelta);

		// Should have moved a total of SeekDelta * 2 bytes
		file.Seek(SeekDelta, IO::SeekMethod::Current);
		CHECK(file.Position() == SeekDelta * 2);
	}

	TEST_FIXTURE(ReaderTestFixture, FileReader_SeekEnd)
	{
		// Should be SeekDelta bytes remaining at the end
		file.Seek(-SeekDelta, IO::SeekMethod::End);
		CHECK(file.Position() == static_cast<int>(file.Size() - SeekDelta));
	}

	TEST_FIXTURE(ReaderTestFixture, FileReader_ReadFullThrows)
	{
		file.Seek(-static_cast<long>(SeekDelta * 1.5), IO::SeekMethod::End);
		uint8_t buffer[SeekDelta];

		// First one should work
		file.ReadFull(buffer, SeekDelta);

		// Second one shouldn't
		CHECK_THROW(file.ReadFull(buffer, SeekDelta), Err::EndOfFile);
	}

	TEST(FileReader_NoFile)
	{
		IO::FileReader f("This file does not exist");
		CHECK(f.Open() == false);
		//CHECK(f.Error() == IO::OpenErrorCode::FileNotFound);
		CHECK(f.IsOpen() == false);	// Shouldn't be open
	}

	TEST_FIXTURE(ReaderTestFixture, FileReader_OpenMany)
	{
		CHECK_THROW(file.Open(), Err::FileAlreadyOpen);
	}

	TEST(DoFileExist)
	{
		CHECK(IO::DoFileExist(g_datapath + "/data.raw") == true);
		CHECK(IO::DoFileExist("This file should not exist") == false);
	}

	TEST_FIXTURE(ReaderAsciiFixture, ReadBytes)
	{
		std::string txt;
		txt.reserve(27);
		char c;
		for(int i = 0; i < 27; ++i)
		{
			CHECK(file.Read(&c, 1, 1) == 1);
			txt.push_back(c);
		}

		CHECK(txt == "This is a simple text file.");
		CHECK(file.Read(&c, 1, 1) == 0);
	}

	TEST_FIXTURE(ReaderAsciiFixture, ReadByteSeek)
	{
		std::string txt;
		txt.reserve(27);
		char c;
		for(int i = 0; i < 27; ++i)
		{
			CHECK(file.Read(&c, 1, 1) == 1);
			txt.push_back(c);
		}

		CHECK(txt == "This is a simple text file.");
		CHECK(file.Read(&c, 1, 1) == 0);
	}

	TEST_FIXTURE(ReaderAsciiFixture, ReadMuch)
	{
		char c[200];
		CHECK(file.Read(c, 1, 100) == 27);

		for(int i = 0; i < 27; ++i)
		{
			bool isEqual = (content.at(i) == c[i]);
			CHECK(isEqual);
			if (!isEqual) return;
		}

		CHECK(file.Read(c, 1, 1) == 0);
	}


	TEST_FIXTURE(ReaderTestFixture, ReadPosition)
	{
		uint8_t c;
		for (int i = 0; i < 12288; ++i)
		{
			bool couldRead = (file.Read(&c, 1, 1) == 1);
			bool correctPosition = (file.Position() == (i + 1));
			CHECK(couldRead);
			CHECK(correctPosition);
			if (couldRead == false || correctPosition == false) break;
		}
	}

	/*TEST(FileAccessViolation)
	{
		const char* filename = L"file_access_test.txt";
		IO::FileWriter writer;
		CHECK(writer.Open(filename) == true);

		IO::FileReader reader(filename);
		CHECK(reader.Open() == false);
		CHECK(reader.Error() == IO::OpenErrorCode::AccessViolation);
	}*/
}
