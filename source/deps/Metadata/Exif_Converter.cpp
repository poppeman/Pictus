#include "Exif_Converter.h"

namespace Metadata {
	namespace Exif {
		void Converter::AddTag(FieldIdentifier docTag, TagName exifTag) {
			if (m_in.Ascii.find(exifTag) != m_in.Ascii.end()) {
				m_out->Field[docTag].reset(new SimpleValue<std::string>(m_in.Ascii[exifTag]));
			}
			if (m_in.Rational.find(exifTag) != m_in.Rational.end() && m_in.Rational[exifTag].size() == 1) {
				m_out->Field[docTag].reset(new SimpleValue<Rational>(m_in.Rational[exifTag].at(0)));
			}
			if (m_in.U16.find(exifTag) != m_in.U16.end()) {
				m_out->Field[docTag].reset(new SimpleValue<int>(m_in.U16[exifTag]));
			}
		}

		void Converter::AddTagUnit(FieldIdentifier docTag, TagName exifTag, std::string unit) {
			if (m_in.Rational.find(exifTag) != m_in.Rational.end() && m_in.Rational[exifTag].size() == 1) {
				m_out->Field[docTag].reset(new SimpleUnitValue<Rational>(m_in.Rational[exifTag].at(0), unit));
			}
			if (m_in.U16.find(exifTag) != m_in.U16.end()) {
				m_out->Field[docTag].reset(new SimpleUnitValue<int>(m_in.U16[exifTag], unit));
			}
		}

		void Converter::AddTagUnit(FieldIdentifier docTag, TagName exifTag, std::string prefix, std::string unit) {
			if (m_in.Rational.find(exifTag) != m_in.Rational.end() && m_in.Rational[exifTag].size() == 1) {
				m_out->Field[docTag].reset(new SimpleUnitValue<Rational>(m_in.Rational[exifTag].at(0), prefix, unit));
			}
			if (m_in.U16.find(exifTag) != m_in.U16.end()) {
				m_out->Field[docTag].reset(new SimpleUnitValue<int>(m_in.U16[exifTag], prefix, unit));
			}
		}

		void Converter::AddGeographicalAxis(FieldIdentifier docTag, TagName coordTag, TagName referenceTag, GeographicalAxis axis) {
			if (m_in.Ascii.find(referenceTag) == m_in.Ascii.end()) {
				return;
			}

			if (m_in.Rational.find(coordTag) != m_in.Rational.end() & !m_in.Rational[coordTag].empty()) {
				auto& tag = m_in.Rational[coordTag];
				auto ref = m_in.Ascii[referenceTag];

				double out = tag.at(0).ToFloat();
				double seconds = 0;
				if (tag.size() >= 2) {
					seconds += tag.at(1).ToFloat() * 60.0;
				}
				if (tag.size() >= 3) {
					seconds += tag.at(2).ToFloat();
				}
				out += (seconds / 3600.0);

				if (ref == "S" || ref == "W") {
					out = -out;
				}

				m_out->Field[docTag].reset(new GeographicalCoordinateValue(out, axis));
			}
		}

		void Converter::Flatten() {
			AddResolution<std::vector<Rational>, uint16_t>(FieldIdentifier::XResolution, m_in.Rational.find(TagName::XResolution), m_in.Rational.end(), m_in.U16.find(TagName::ResolutionUnit), m_in.U16.end());
			AddResolution<std::vector<Rational>, uint16_t>(FieldIdentifier::YResolution, m_in.Rational.find(TagName::YResolution), m_in.Rational.end(), m_in.U16.find(TagName::ResolutionUnit), m_in.U16.end());
			AddTagUnit(FieldIdentifier::ExposureTime, TagName::ExposureTime, "s");

			if (m_in.U16[TagName::DigitalZoomRatio] > 0) {
				AddTagUnit(FieldIdentifier::DigitalZoom, TagName::DigitalZoomRatio, "x");
			}

			AddTag(FieldIdentifier::Orientation, TagName::Orientation);

			if (m_in.Rational.find(TagName::FocalLength) != m_in.Rational.end() && m_in.Rational[TagName::FocalLength].size() == 1) {
				m_out->Field[FieldIdentifier::FocalLength].reset(new SimpleUnitValue<float>(std::floor(0.5 + m_in.Rational[TagName::FocalLength].at(0).ToFloat()), "mm"));
			}
			if (m_in.Rational.find(TagName::ExposureBias) != m_in.Rational.end() && m_in.Rational[TagName::FocalLength].size() == 1) {
				m_out->Field[FieldIdentifier::ExposureBias].reset(new SimpleUnitValue<float>(m_in.Rational[TagName::ExposureBias].at(0).ToFloat(), "step"));
			}
			if (m_in.Rational.find(TagName::FNumber) != m_in.Rational.end() && m_in.Rational[TagName::FNumber].size() == 1) {
				std::stringstream ss;
				ss << std::setprecision(2) << m_in.Rational[TagName::FNumber].at(0).ToFloat();
				auto str = ss.str();
				m_out->Field[FieldIdentifier::FNumber].reset(new SimpleUnitValue<std::string>(str, "f/", ""));
			}

			AddTag(FieldIdentifier::Make, TagName::Make);
			AddTag(FieldIdentifier::Description, TagName::ImageDescription);
			AddTag(FieldIdentifier::Comment, TagName::UserComment);
			AddTag(FieldIdentifier::Software, TagName::Software);
			AddTag(FieldIdentifier::Model, TagName::Model);
			AddTag(FieldIdentifier::Copyright, TagName::Copyright);

			AddTag(FieldIdentifier::WhitePoint, TagName::WhitePoint);
			AddTag(FieldIdentifier::YCbCrCoefficients, TagName::YCbCrCoefficients);
			AddTag(FieldIdentifier::ReferenceBlackWhite, TagName::ReferenceBlackWhite);
			AddTag(FieldIdentifier::FocalPlaneXResolution, TagName::FocalPlaneXResolution);
			AddTag(FieldIdentifier::FocalPlaneYResolution, TagName::FocalPlaneYResolution);
			if (m_in.Rational.find(TagName::MaxAperture) != m_in.Rational.end() && m_in.Rational[TagName::MaxAperture].size() == 1) {
				float fval = pow(sqrt(2), m_in.Rational[TagName::MaxAperture].at(0).ToFloat());
				std::stringstream ss;
				ss << std::setprecision(2) << fval;
				auto str = ss.str();
				m_out->Field[FieldIdentifier::MaxAperture].reset(new SimpleUnitValue<std::string>(str, "f/", ""));
			}
			AddTagUnit(FieldIdentifier::ISOSpeedRating, TagName::IsoSpeedRatings, "ISO-", "");

			if (m_in.U16.find(TagName::LightSource) != m_in.U16.end()) {
				m_out->Field[FieldIdentifier::LightSource] = std::shared_ptr<Value>(new SimpleValue<LightSource>(LightSource(m_in.U16[TagName::LightSource])));
			}

			if (m_in.U16.find(TagName::ExposureProgram) != m_in.U16.end()) {
				m_out->Field[FieldIdentifier::ExposureProgram] = std::shared_ptr<Value>(new SimpleValue<ExposureProgram>(ExposureProgram(m_in.U16[TagName::ExposureProgram])));
			}

			AddGeographicalAxis(FieldIdentifier::Latitude, TagName::Latitude, TagName::LatitudeRef, GeographicalAxis::Latitude);
			AddGeographicalAxis(FieldIdentifier::Longitude, TagName::Longitude, TagName::LongitudeRef, GeographicalAxis::Longitude);
		}

	}
}
