#include "orz/fileops.h"
#include "orz/types.h"

#include <UnitTest++/UnitTest++.h>

SUITE(PathString)
{
	TEST(get_path)
	{
#ifdef _WIN32
		CHECK_EQUAL("c:\\temp\\lol\\", IO::GetPath("c:\\temp/lol/dsfargeg.txt"));
		CHECK_EQUAL("c:\\temp\\", IO::GetPath("c:\\temp\\\\\\\\"));
#else
		CHECK_EQUAL("/temp/lol/", IO::GetPath("/temp/lol/dsfargeg.txt"));
		CHECK_EQUAL("/temp/", IO::GetPath("/temp////"));
#endif
	}

	TEST(get_file)
	{
#ifdef _WIN32
		std::string s1("c:\\temp/lol/dsfargeg.txt.lol");
		std::string s2("c:\\temp/some_periods.in_here\\dsfargeg.txt.lol");
#else
		std::string s1("/temp/lol/dsfargeg.txt.lol");
		std::string s2("/temp/some_periods.in_here/dsfargeg.txt.lol");
#endif
		CHECK_EQUAL("dsfargeg.txt.lol", IO::GetFile(s1));
		CHECK_EQUAL("dsfargeg.txt.lol", IO::GetFile(s2));
	}

	TEST(get_title)
	{
#ifdef _WIN32
		std::string s1("c:\\temp/lol/dsfargeg.txt.lol");
		std::string s2("c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
#else
		std::string s1("/temp/lol/dsfargeg.txt.lol");
		std::string s2("/temp/some_periods.in_here/dsfargeg.txt.lol");
#endif
		CHECK_EQUAL("dsfargeg.txt", IO::GetTitle(s1));
		CHECK_EQUAL("dsfargeg.txt", IO::GetTitle(s2));
	}

	TEST(get_ext)
	{
#ifdef _WIN32
		std::string s1(u8"c:\\temp/lol/dsfargeg.txt.lol");
		std::string s2(u8"c:\\temp/some_periods.in_here/dsfargeg.txt.lol");
		std::string s3(u8"c:\\temp/some_periods.in_here/nofileextension");
#else
		std::string s1(u8"/temp/lol/dsfargeg.txt.lol");
		std::string s2(u8"/temp/some_periods.in_here/dsfargeg.txt.lol");
		std::string s3(u8"/temp/some_periods.in_here/nofileextension");
#endif
		CHECK_EQUAL("lol", IO::GetExtension(s1));
		CHECK_EQUAL("lol", IO::GetExtension(s2));
		CHECK_EQUAL("", IO::GetExtension(s3));
	}

	TEST(get_ext_utf8)
	{
#ifdef _WIN32
		const char *s1 = u8"c:\\temp/日本語のインターフェース.tga";
		const char *s2 = u8"c:\\temp/日本語のインターフェース.tga/nof";
		const char *s3 = u8"c:\\temp/日本語のインターフェース.tga\\nof";
#else
		const char *s1 = u8"/temp/日本語のインターフェース.tga";
		const char *s2 = u8"/temp/日本語のインターフェース.tga/nof";
		const char *s3 = u8"/temp/日本語のインターフェース.tga/nof";
#endif
		CHECK_EQUAL("tga", IO::GetExtension(s1));
		CHECK_EQUAL("", IO::GetExtension(s2));
		CHECK_EQUAL("", IO::GetExtension(s3));
	}
}
