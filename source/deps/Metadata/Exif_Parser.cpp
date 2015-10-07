#include "Exif_Parser.h"
#include "Exif_Types.h"
#include "../orz/file_reader.h"
#include "../orz/stream_mem.h"
#include <cstdint>
#include <iomanip>

namespace Metadata {
	namespace Exif {
		uint16_t ReadUint16(IO::FileReader::Ptr ms, ByteOrder o, size_t position) {
			// Assumes host with LSB endianness
			ms->Seek(position, IO::SeekMethod::Begin);
			if (o == ByteOrder::Motorola) {
				// Network byte order.
				return IO::ReadNet16(ms);
			}
			return IO::ReadLE16(ms);
		}

		uint16_t ReadUint16(IO::FileReader::Ptr ms, ByteOrder o) {
			return ReadUint16(ms, o, ms->Position());
		}

		uint32_t ReadUint32(IO::FileReader::Ptr ms, ByteOrder o) {
			// Assumes host with LSB endianness
			if (o == ByteOrder::Motorola) {
				// Network byte order.
				return IO::ReadNet32(ms);
			}
			return IO::ReadLE32(ms);
		}

		uint16_t FirstUint16(uint32_t data, ByteOrder o) {
			if(o == ByteOrder::Motorola)
				data = data >> 16;
			return data & 0xffff;
		}

		uint16_t SecondUint16(uint32_t data, ByteOrder o) {
			if(o == ByteOrder::Intel)
				data = data >> 16;
			return data & 0xffff;
		}

		std::string ReadAscii(IO::FileReader::Ptr ms, size_t offset, size_t components) {
			std::stringstream ss;
			if(components <= 4) {
				for (int i = 0; i < components; ++i) {
					char c;
					ms->ReadFull(&c, 1);
					ss << c;
				}
			}
			else {
				for(auto i=0;i<components;++i) {
					ms->Seek(offset + i, IO::SeekMethod::Begin);
					char C;
					ms->ReadFull(&C, 1);
					ss << 1;
				}
			}
			return ss.str();
		}

		Metadata::Rational ReadRational(IO::FileReader::Ptr ms, ByteOrder o, size_t position) {
			// Assumes unsigned rationals
			ms->Seek(position, IO::SeekMethod::Begin);
			auto num = ReadUint32(ms, o);
			auto nom = ReadUint32(ms, o);
			return Metadata::Rational(num, nom);
		}

		void ReadIfd(IO::FileReader::Ptr ms, ByteOrder o, ExifDocument& doc) {
			auto num_entries = ReadUint16(ms, o);

			for(auto entry = 0; entry < num_entries; ++entry) {
				auto raw_type = ReadUint16(ms, o);
				auto raw_format = ReadUint16(ms, o);
				auto components = ReadUint32(ms, o);
				auto offset_data = ReadUint32(ms, o); // Offset OR data, depending on size(format) * components
				
				auto name = TagName(raw_type);
				auto format = TagFormat(raw_format);
				auto pos = ms->Position();
				if(format == TagFormat::Ascii) {
					doc.Ascii[name] = ReadAscii(ms, offset_data, components);
				}
				else if(format == TagFormat::URational || format == TagFormat::SRational) {
					for(unsigned i=0;i<components; ++i) {
						doc.Rational[name].push_back(ReadRational(ms, o, offset_data));
						offset_data += 8;
					}
				}
				else if(format == TagFormat::UInt32) {
					ms->Seek(offset_data, IO::SeekMethod::Begin);
					if(name == TagName::SubIfd || name == TagName::GpsIfd)
						ReadIfd(ms, o, doc);
					// Todo: Support UInt32 tags
				}
				else if(format == TagFormat::UInt16) {
					auto data = FirstUint16(offset_data, o);
					// Todo: Support components > 1
					doc.U16[name] = data;
				}
				ms->Seek(pos, IO::SeekMethod::Begin);
			}
		}

		class Converter {
		public:
			template <typename T, typename U>
			void AddResolution(
				FieldIdentifier docTag,
				typename std::map<TagName, T>::iterator value,
				typename std::map<TagName, T>::iterator value_end,
				typename std::map<TagName, U>::iterator unit,
				typename std::map<TagName, U>::iterator unit_end) {
				if(value == value_end)
					return;
				if(value->second.size() != 1)
					return;

				auto tn = ResolutionUnit::Undefined;
				if(unit != unit_end) {
					tn = ResolutionUnit(unit->second);
				}
				auto val = value->second.at(0).ToInteger();
				auto dru = ResolutionUnit(tn);
				m_out->Field[docTag].reset(new ResolutionAxisValue<int>(val, dru));
			}

			void AddTag(FieldIdentifier docTag, TagName exifTag) {
				if(m_in.Ascii.find(exifTag) != m_in.Ascii.end())
					m_out->Field[docTag].reset(new SimpleValue<std::string>(m_in.Ascii[exifTag]));
				if(m_in.Rational.find(exifTag) != m_in.Rational.end() && m_in.Rational[exifTag].size() == 1)
					m_out->Field[docTag].reset(new SimpleValue<Rational>(m_in.Rational[exifTag].at(0)));
				if(m_in.U16.find(exifTag) != m_in.U16.end())
					m_out->Field[docTag].reset(new SimpleValue<int>(m_in.U16[exifTag]));
			}

			void AddTagUnit(FieldIdentifier docTag, TagName exifTag, std::string unit) {
				if(m_in.Rational.find(exifTag) != m_in.Rational.end() && m_in.Rational[exifTag].size() == 1)
					m_out->Field[docTag].reset(new SimpleUnitValue<Rational>(m_in.Rational[exifTag].at(0), unit));
				if(m_in.U16.find(exifTag) != m_in.U16.end())
					m_out->Field[docTag].reset(new SimpleUnitValue<int>(m_in.U16[exifTag], unit));
			}

			void AddTagUnit(FieldIdentifier docTag, TagName exifTag, std::string prefix, std::string unit) {
				if(m_in.Rational.find(exifTag) != m_in.Rational.end() && m_in.Rational[exifTag].size() == 1)
					m_out->Field[docTag].reset(new SimpleUnitValue<Rational>(m_in.Rational[exifTag].at(0), prefix, unit));
				if(m_in.U16.find(exifTag) != m_in.U16.end())
					m_out->Field[docTag].reset(new SimpleUnitValue<int>(m_in.U16[exifTag], prefix, unit));
			}

			void AddGeographicalAxis(FieldIdentifier docTag, TagName coordTag, TagName referenceTag, GeographicalAxis axis) {
				if(m_in.Ascii.find(referenceTag) == m_in.Ascii.end())
					return;

				if(m_in.Rational.find(coordTag) != m_in.Rational.end() & !m_in.Rational[coordTag].empty()) {
					auto& tag = m_in.Rational[coordTag];
					auto ref = m_in.Ascii[referenceTag];

					double out = tag.at(0).ToFloat();
					double seconds = 0;
					if(tag.size() >= 2)
						seconds += tag.at(1).ToFloat() * 60.0;
					if(tag.size() >= 3)
						seconds += tag.at(2).ToFloat();
					out += (seconds / 3600.0);

					if(ref == "S" || ref == "W")
						out = -out;

					m_out->Field[docTag].reset(new GeographicalCoordinateValue(out, axis));
				}
			}

			void Flatten() {
				AddResolution<std::vector<Rational>, uint16_t>(FieldIdentifier::XResolution, m_in.Rational.find(TagName::XResolution), m_in.Rational.end(), m_in.U16.find(TagName::ResolutionUnit), m_in.U16.end());
				AddResolution<std::vector<Rational>, uint16_t>(FieldIdentifier::YResolution, m_in.Rational.find(TagName::YResolution), m_in.Rational.end(), m_in.U16.find(TagName::ResolutionUnit), m_in.U16.end());
				AddTagUnit(FieldIdentifier::ExposureTime, TagName::ExposureTime, "s");

				if(m_in.U16[TagName::DigitalZoomRatio] > 0)
					AddTagUnit(FieldIdentifier::DigitalZoom, TagName::DigitalZoomRatio, "x");

				if(m_in.Rational.find(TagName::FocalLength) != m_in.Rational.end() && m_in.Rational[TagName::FocalLength].size() == 1) {
					m_out->Field[FieldIdentifier::FocalLength].reset(new SimpleUnitValue<float>(std::floor(0.5 + m_in.Rational[TagName::FocalLength].at(0).ToFloat()), "mm"));
				}
				if(m_in.Rational.find(TagName::ExposureBias) != m_in.Rational.end() && m_in.Rational[TagName::FocalLength].size() == 1) {
					m_out->Field[FieldIdentifier::ExposureBias].reset(new SimpleUnitValue<float>(m_in.Rational[TagName::ExposureBias].at(0).ToFloat(), "step"));
				}
				if(m_in.Rational.find(TagName::FNumber) != m_in.Rational.end() && m_in.Rational[TagName::FNumber].size() == 1) {
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
				if(m_in.Rational.find(TagName::MaxAperture) != m_in.Rational.end() && m_in.Rational[TagName::MaxAperture].size() == 1) {
					float fval = pow(sqrt(2), m_in.Rational[TagName::MaxAperture].at(0).ToFloat());
					std::stringstream ss;
					ss << std::setprecision(2) << fval;
					auto str = ss.str();
					m_out->Field[FieldIdentifier::MaxAperture].reset(new SimpleUnitValue<std::string>(str, "f/", ""));
				}
				AddTagUnit(FieldIdentifier::ISOSpeedRating, TagName::IsoSpeedRatings, "ISO-", "");

				if(m_in.U16.find(TagName::LightSource) != m_in.U16.end()) {
					m_out->Field[FieldIdentifier::LightSource] = std::shared_ptr<Value>(new SimpleValue<LightSource>(LightSource(m_in.U16[TagName::LightSource])));
				}

				if(m_in.U16.find(TagName::ExposureProgram) != m_in.U16.end()) {
					m_out->Field[FieldIdentifier::ExposureProgram] = std::shared_ptr<Value>(new SimpleValue<ExposureProgram>(ExposureProgram(m_in.U16[TagName::ExposureProgram])));
				}

				AddGeographicalAxis(FieldIdentifier::Latitude, TagName::Latitude, TagName::LatitudeRef, GeographicalAxis::Latitude);
				AddGeographicalAxis(FieldIdentifier::Longitude, TagName::Longitude, TagName::LongitudeRef, GeographicalAxis::Longitude);
			};

			Converter(ExifDocument& in, std::shared_ptr<Document> out):m_in(in), m_out(out) {}

			ExifDocument& m_in;
			std::shared_ptr<Document> m_out;
		};

		std::shared_ptr<Document> Decode(unsigned char* data, size_t length) {
			auto doc = std::shared_ptr<Document>();

			auto ms = std::make_shared<IO::FileReader>(std::make_shared<IO::StreamMemory>(data, length));

			ByteOrder bo = ByteOrder::Undefined;
			{
				uint16_t order;
				ms->ReadFull(&order, 2);

				if(order == 0x4949)
					bo = ByteOrder::Intel;

				if(order == 0x4d4d)
					bo = ByteOrder::Motorola;
			}
			if(bo == ByteOrder::Undefined)
				return doc;

			if(ReadUint16(ms, bo) != 0x2a)
				return doc;

			auto ifd_offset = ReadUint32(ms, bo);
			// Todo: Seek to correct position if ifd_offset != 8

			doc.reset(new Document);
			ExifDocument tc;
			ReadIfd(ms, bo, tc);
			Converter c(tc, doc);
			c.Flatten();

			return doc;
		}
	}
}
