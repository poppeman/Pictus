#ifndef IMAGE_EXIF_TYPES_H
#define IMAGE_EXIF_TYPES_H

#include <map>
#include <vector>
#include <cstdint>
#include "Metadata_Value.h"

namespace Metadata {
	namespace Exif {
		enum class ByteOrder {
			Undefined,
			Intel,
			Motorola
		};

		enum class TagName {
			// IFD0 tags
			ImageDescription = 0x010e,
			Make = 0x010f,
			Model = 0x0110,
			Orientation = 0x0112,
			XResolution = 0x011a,
			YResolution = 0x011b,
			ResolutionUnit = 0x0128,

			Software = 0x0131,
			DateTime = 0x0132,
			WhitePoint = 0x013e,
			PrimaryChromaticities = 0x013f,
			YCbCrCoefficients = 0x0211,
			YCbCrPositioning = 0x0213,
			ReferenceBlackWhite = 0x0214,
			Copyright = 0x8298,

			// Sub IFD
			ExposureTime = 0x829a,
			FNumber = 0x829d,
			ExposureProgram = 0x8822,
			IsoSpeedRatings = 0x8827,
			ExifVersion = 0x9000,
			DateTimeOriginal = 0x9003,
			DateTimeDigitized = 0x9004,
			ComponentConfiguration = 0x9101, // Unknown use
			CompressedBitsPerPixel = 0x9102,
			Brightness = 0x9203,
			ExposureBias = 0x9204,
			MaxAperture = 0x9205,
			SubjectDistance = 0x9206,
			MeteringMode = 0x9207,
			LightSource = 0x9208,
			Flash = 0x9209,
			FocalLength = 0x920a,
			MakerNote = 0x927c, // Maker dependent data, not handled,
			UserComment = 0x9286,
			FlashPixVersion = 0xa000, // Unknown use
			ColorSpace = 0xa001,
			ExifImageWidth = 0xa002,
			ExifImageHeight = 0xa003,
			RelatedSoundFile = 0xa004,
			ExifInteroperabilityOffset = 0xa005, // Unknown use
			FocalPlaneXResolution = 0xa20e,
			FocalPlaneYResolution = 0xa20f,
			FocalPlaneResolutionUnit = 0xa210,
			SensingMethod = 0xa217,
			FileSource = 0xa300,
			SceneType = 0xa301,
			DigitalZoomRatio = 0xa404,
			SubIfd = 0x8769,


			GpsIfd = 0x8825,
			LatitudeRef = 0x001,
			Latitude = 0x0002,
			LongitudeRef = 0x0003,
			Longitude = 0x0004,
			AltitudeRef = 0x0005,
			Altitude = 0x0006,

		};

		enum class TagFormat {
			UInt8 = 1,
			Ascii = 2,
			UInt16 = 3,
			UInt32 = 4,
			URational = 5,
			SInt8 = 6,
			Undefined = 7,
			SInt16 = 8,
			SInt32 = 9,
			SRational = 10,
			Float = 11,
			Double = 12
		};

		struct ExifDocument {
			std::map<TagName, uint16_t> U16;
			std::map<TagName, std::vector<Rational>> Rational;
			std::map<TagName, std::string> Ascii;
		};
	}
}

#endif
