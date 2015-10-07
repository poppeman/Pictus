#ifndef IMAGE_METADATA_TYPES_H
#define IMAGE_METADATA_TYPES_H

namespace Metadata {
	enum class FieldIdentifier {
		// Strings
		Title,
		Author,
		Description,
		Copyright,
		Software,
		Disclaimer,
		Warning,
		Source,
		Comment,
		Make,
		Model,

		// Rationals
		FNumber,
		FocalLength,
		WhitePoint,
		PrimaryChromaticities,
		YCbCrCoefficients,
		ReferenceBlackWhite,
		XResolution,
		YResolution,
		FocalPlaneXResolution,
		FocalPlaneYResolution,
		DigitalZoom,
		ExposureTime,
		MaxAperture,
		ExposureBias,

		// String arrays
		SubjectTags,

		// GPS
		Latitude,
		Longitude,

		// Unsigned shorts
		ISOSpeedRating,
		YCbCrPositioning,

		// Other
		LightSource,
		ExposureProgram
	};

	enum class ResolutionUnit {
		Undefined,
		NoUnit = 1,
		Inch = 2,
		Centimeter = 3
	};

	enum class LightSource {
		Auto = 0,
		Daylight = 1,
		Fluorescent = 2,
		Tungsten = 3,
		Flash = 4,
		FineWeather = 9,
		CloudyWeather = 10,
		Shade = 11,
		DaylightFluorescent = 12,
		DayWhiteFluorescent = 13,
		CoolWhiteFluorescent = 14,
		WhiteFluorescent = 15,
		StandardLightA = 17,
		StandardLightB = 18,
		StandardLightC = 19,
		D55 = 20,
		D65 = 21,
		D75 = 22,
		D50 = 23,
		IsoStudioTungsten = 24,
		Other = 255
	};

	enum ExposureProgram {
		Undefined = 0,
		Manual = 1,
		Normal = 2,
		AperturePriority = 3,
		ShutterPriority = 4,
		Creative = 5,
		Action = 6,
		Portrait = 7,
		Landscape = 8
	};
}

#endif
