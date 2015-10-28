#ifndef METADATA_EXIF_CONVERTER_H
#define METADATA_EXIF_CONVERTER_H

#include "Metadata_Document.h"
#include "Metadata_Types.h"
#include "Metadata_Value.h"
#include "Exif_Types.h"
#include <map>
#include <memory>
#include <iomanip>

namespace Metadata {
	namespace Exif {
		class Converter {
		public:
			template <typename T, typename U>
			void AddResolution(
				FieldIdentifier docTag,
				typename std::map<TagName, T>::iterator value,
				typename std::map<TagName, T>::iterator value_end,
				typename std::map<TagName, U>::iterator unit,
				typename std::map<TagName, U>::iterator unit_end) {
				if (value == value_end)
					return;
				if (value->second.size() != 1)
					return;

				auto tn = ResolutionUnit::Undefined;
				if (unit != unit_end) {
					tn = ResolutionUnit(unit->second);
				}
				auto val = value->second.at(0).ToInteger();
				auto dru = ResolutionUnit(tn);
				m_out->Field[docTag].reset(new ResolutionAxisValue<int>(val, dru));
			}

			void AddTag(FieldIdentifier docTag, TagName exifTag);

			void AddTagUnit(FieldIdentifier docTag, TagName exifTag, std::string unit);

			void AddTagUnit(FieldIdentifier docTag, TagName exifTag, std::string prefix, std::string unit);

			void AddGeographicalAxis(FieldIdentifier docTag, TagName coordTag, TagName referenceTag, GeographicalAxis axis);

			void Flatten();

			Converter(ExifDocument& in, std::shared_ptr<Document> out) :m_in(in), m_out(out) {}

			ExifDocument& m_in;
			std::shared_ptr<Document> m_out;
		};
	}
}

#endif
