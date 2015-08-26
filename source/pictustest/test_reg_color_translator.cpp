#include "StdAfx.h"
#include "Pictus/reg_color_translator.h"

SUITE(ColorTranslatorSuite) {
	using namespace Reg::Internal;
	TEST(ConvertEmpty) {
		ColorTranslator s;
		CHECK(s.get_value("").is_initialized() == false);
	}

	TEST(ConvertNumeric) {
		ColorTranslator s;
		auto ret = s.get_value("14675691");
		CHECK(ret.is_initialized());
		CHECK(ret.get() == Img::Color(0, 0xdf, 0xee, 0xeb));
	}

	TEST(ConvertHexLong) {
		ColorTranslator s;
		auto ret = s.get_value("#9abcde");
		CHECK(ret.is_initialized());
		CHECK(ret.get() == Img::Color(0, 0x9a, 0xbc, 0xde));
	}

	TEST(PutBlack) {
		ColorTranslator s;
		auto ret = s.put_value({ 0, 0, 0, 0 });
		CHECK(ret.is_initialized());
		CHECK(ret.get() == "#000000");
	}

	TEST(PutConvertSingle) {
		ColorTranslator s;
		Img::Color col = { 0, 0x23, 0x9b, 0xfa };
		auto put = s.put_value(col);
		CHECK(put.is_initialized());
		auto get = s.get_value(put.get());
		CHECK(get.is_initialized());
		CHECK(get.get() == col);
	}
}
