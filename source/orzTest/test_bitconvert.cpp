#include "orz/bitconvert.h"
#include <UnitTest++/UnitTest++.h>

SUITE(BitConvert)
{
	/*

		Nibble

	*/

	TEST(NibbleMatch)
	{
		uint8_t dest[2];
		uint8_t source = 0x98;
		Util::NibblesToBytes(dest, &source, 2);
		CHECK(dest[0] == 0x09);
		CHECK(dest[1] == 0x08);
	}

	TEST(NibbleNotMatch)
	{
		uint8_t dest[3];
		uint8_t source[2] = { 0x98, 0x23 };
		Util::NibblesToBytes(dest, source, 3);
		CHECK(dest[0] == 0x09);
		CHECK(dest[1] == 0x08);
		CHECK(dest[2] == 0x02);
	}

	TEST(NibbleSingle)
	{
		uint8_t dest;
		uint8_t source = 0xCD;
		Util::NibblesToBytes(&dest, &source, 1);
		CHECK(dest == 0x0C);
	}

	TEST(NibbleNullNum)
	{
		uint8_t dest = 0x33;
		uint8_t source = 0xff;
		Util::NibblesToBytes(&dest, &source, 0);
		CHECK(dest == 0x33);
	}

	/*

	Crumb

	*/


	TEST(CrumbMatch)
	{
		uint8_t dest[4];
		uint8_t source = 0x98;
		Util::CrumbsToBytes(dest, &source, 4);
		CHECK(dest[0] == 0x02);
		CHECK(dest[1] == 0x01);
		CHECK(dest[2] == 0x02);
		CHECK(dest[3] == 0x00);
	}

	TEST(CrumbNotMatch)
	{
#pragma warning(push)
#pragma warning(disable: 6385)
		uint8_t dest[4] = { 0, 0, 0, 0xff};
		uint8_t source = 0x98;
		Util::CrumbsToBytes(dest, &source, 3);
		CHECK(dest[0] == 0x02);
		CHECK(dest[1] == 0x01);
		CHECK(dest[2] == 0x02);
		CHECK(dest[3] == 0xff);
#pragma warning(pop)
	}

	TEST(CrumbSingle)
	{
		uint8_t dest;
		uint8_t source = 0xCD;
		Util::CrumbsToBytes(&dest, &source, 1);
		CHECK(dest == 0x03);
	}

	TEST(CrumbNullNum)
	{
		uint8_t dest = 0x33;
		uint8_t source = 0xff;
		Util::CrumbsToBytes(&dest, &source, 0);
		CHECK(dest == 0x33);
	}


	/*

	Bit

	*/


	TEST(BitMatch)
	{
		uint8_t dest[8];
		uint8_t source = 0x98;
		Util::BitsToBytes(dest, &source, 8);
		CHECK(dest[0] == 0x01);
		CHECK(dest[1] == 0x00);
		CHECK(dest[2] == 0x00);
		CHECK(dest[3] == 0x01);
		CHECK(dest[4] == 0x01);
		CHECK(dest[5] == 0x00);
		CHECK(dest[6] == 0x00);
		CHECK(dest[7] == 0x00);
	}

	TEST(BitNotMatch)
	{
#pragma warning(push)
#pragma warning(disable: 6385)
		uint8_t dest[5] = { 0, 0, 0, 0xff};
		uint8_t source = 0x98;
		Util::BitsToBytes(dest, &source, 3);
		CHECK(dest[0] == 0x01);
		CHECK(dest[1] == 0x00);
		CHECK(dest[2] == 0x00);
		CHECK(dest[3] == 0xff);
#pragma warning(pop)
	}

	TEST(BitSingle)
	{
		uint8_t dest = 0;
		uint8_t source = 0xCD;
		Util::BitsToBytes(&dest, &source, 1);
		CHECK(dest == 0x01);
	}

	TEST(BitNullNum)
	{
		uint8_t dest = 0x33;
		uint8_t source = 0xff;
		Util::BitsToBytes(&dest, &source, 0);
		CHECK(dest == 0x33);
	}
}
