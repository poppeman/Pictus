#include "StdAfx.h"

#include "orz/fileops.h"

SUITE(PathString)
{
	TEST(get_path)
	{
		CHECK(IO::GetPath(TX("c:\\temp/lol/dsfargeg.txt")) == TX("c:\\temp\\lol\\"));
		CHECK(IO::GetPath(TX("c:\\temp\\\\\\\\")) == L"c:\\temp\\");
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
		std::wstring s1(L"c:\\temp/lol/dsfargeg.txt.lol");
		CHECK(IO::GetExtension(s1) == L"lol");

		std::wstring s2(L"c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
		CHECK(IO::GetExtension(s2) == L"lol");

		std::wstring s3(L"c:\\temp/some_periods.in_here/nofileextension");
		CHECK(IO::GetExtension(s3) == L"");
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

SUITE(MaxMin) {
	using Util::Max;
	using Util::Min;

	TEST(TripleMax) {
		CHECK_EQUAL(9, Max(1, 3, 9));
		CHECK_EQUAL(9, Max(9, 3, 1));
		CHECK_EQUAL(9, Max(3, 9, 1));
	}

	TEST(TripleMin) {
		CHECK_EQUAL(1, Min(1, 3, 9));
		CHECK_EQUAL(1, Min(9, 3, 1));
		CHECK_EQUAL(1, Min(3, 9, 1));
	}

}