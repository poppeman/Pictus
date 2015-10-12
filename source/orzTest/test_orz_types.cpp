#include "orz/fileops.h"
#include "orz/types.h"

#include <UnitTest++/UnitTest++.h>

SUITE(PathString)
{
	TEST(get_path)
	{
		CHECK_EQUAL("c:\\temp\\lol\\", IO::GetPath("c:\\temp/lol/dsfargeg.txt"));
		CHECK_EQUAL("c:\\temp\\", IO::GetPath("c:\\temp\\\\\\\\"));
	}

	TEST(get_file)
	{
		std::string s1("c:\\temp/lol/dsfargeg.txt.lol");
		CHECK_EQUAL("dsfargeg.txt.lol", IO::GetFile(s1));

		std::string s2("c:\\temp/some_periods.in_here\\dsfargeg.txt.lol");
		CHECK_EQUAL("dsfargeg.txt.lol", IO::GetFile(s2));
	}

	TEST(get_title)
	{
		std::string s1("c:\\temp/lol/dsfargeg.txt.lol");
		CHECK_EQUAL("dsfargeg.txt", IO::GetTitle(s1));
		std::string s2("c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
		CHECK_EQUAL("dsfargeg.txt", IO::GetTitle(s2));
	}

	TEST(get_ext)
	{
		std::string s1(u8"c:\\temp/lol/dsfargeg.txt.lol");
		CHECK_EQUAL("lol", IO::GetExtension(s1));

		std::string s2(u8"c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
		CHECK_EQUAL("lol", IO::GetExtension(s2));

		std::string s3(u8"c:\\temp/some_periods.in_here/nofileextension");
		CHECK_EQUAL("", IO::GetExtension(s3));
	}

	TEST(get_ext_utf8)
	{
		CHECK_EQUAL("tga", IO::GetExtension(u8"c:\\temp/日本語のインターフェース.tga"));
		CHECK_EQUAL("", IO::GetExtension(u8"c:\\temp/日本語のインターフェース.tga/nof"));
		CHECK_EQUAL("", IO::GetExtension(u8"c:\\temp/日本語のインターフェース.tga\\nof"));
	}
}

SUITE(StringOps)
{
	TEST(to_upper)
	{
		std::string s1("MiXEd caSe");
		CHECK_EQUAL("MIXED CASE", ToUpper(s1));
	}

	TEST(to_lower)
	{
		std::string s1("MiXEd caSe");
		CHECK_EQUAL("mixed case", ToLower(s1));
	}
}
