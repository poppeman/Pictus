#include "orz/fileops.h"
#include "orz/types.h"

#include <UnitTest++/UnitTest++.h>

SUITE(PathString)
{
	TEST(get_path)
	{
		CHECK(IO::GetPath(L"c:\\temp/lol/dsfargeg.txt") == L"c:\\temp\\lol\\");
		CHECK(IO::GetPath(L"c:\\temp\\\\\\\\") == L"c:\\temp\\");
	}

	TEST(get_file)
	{
		std::wstring s1(L"c:\\temp/lol/dsfargeg.txt.lol");
		CHECK(IO::GetFile(s1) == L"dsfargeg.txt.lol");

		std::wstring s2(L"c:\\temp/some_periods.in_here\\dsfargeg.txt.lol");
		CHECK(IO::GetFile(s2) == L"dsfargeg.txt.lol");
	}

	TEST(get_title)
	{
		std::wstring s1(L"c:\\temp/lol/dsfargeg.txt.lol");
		CHECK(IO::GetTitle(s1) == L"dsfargeg.txt");
		std::wstring s2(L"c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
		CHECK(IO::GetTitle(s2) == L"dsfargeg.txt");
	}

	TEST(get_ext)
	{
		std::string s1(u8"c:\\temp/lol/dsfargeg.txt.lol");
		CHECK(IO::GetExtension(s1) == "lol");

		std::string s2(u8"c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
		CHECK(IO::GetExtension(s2) == "lol");

		std::string s3(u8"c:\\temp/some_periods.in_here/nofileextension");
		CHECK(IO::GetExtension(s3) == "");
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
		std::wstring s1(L"MiXEd caSe");
		CHECK(ToUpper(s1)==std::wstring(L"MIXED CASE"));
	}

	TEST(to_lower)
	{
		std::wstring s1(L"MiXEd caSe");
		CHECK(ToLower(s1)==std::wstring(L"mixed case"));
	}
}
