#include "main.h"
#include "orz/file_reader.h"
#include "UnitTest++/UnitTest++.h"
#include "Metadata/Exif_Parser.h"

using namespace Metadata;

SUITE(ExifTest)
{
	/*
		Tests a fairly standard EXIF file, taken with an Olympus E-PL1
	*/
	TEST(SimpleMetadata)
	{
		auto file = std::make_shared<IO::FileReader>(g_datapath + "/Metadata/P8290215.exif");
		auto data = IO::ReadAll(file);
		auto doc = Exif::Decode(&data[0], data.size());
		CHECK(doc != nullptr);
		CHECK_EQUAL(200, doc->Field[FieldIdentifier::ISOSpeedRating]->ToInteger());
		CHECK_EQUAL(std::string("OLYMPUS IMAGING CORP."), doc->Field[FieldIdentifier::Make]->ToString());
		CHECK_EQUAL(std::string("OLYMPUS DIGITAL CAMERA"), doc->Field[FieldIdentifier::Description]->ToString());
	}
}
