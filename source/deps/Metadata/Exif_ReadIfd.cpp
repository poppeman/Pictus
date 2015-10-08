#include "Exif_IO.h"
#include "Exif_ReadIfd.h"
#include "Exif_Types.h"
#include "Metadata_Types.h"
#include "Metadata_Value.h"
#include <boost/algorithm/string.hpp>

namespace Metadata {
	namespace Exif {
		void ReadIfd(IO::FileReader::Ptr ms, ByteOrder o, ExifDocument& doc)
		{
			auto num_entries = ReadUint16(ms, o);

			for (auto entry = 0; entry < num_entries; ++entry) {
				auto raw_type = ReadUint16(ms, o);
				auto raw_format = ReadUint16(ms, o);
				auto components = ReadUint32(ms, o);
				auto offset_data = ReadUint32(ms, o); // Offset OR data, depending on size(format) * components

				auto name = TagName(raw_type);
				auto format = TagFormat(raw_format);
				auto pos = ms->Position();
				if (format == TagFormat::Ascii) {
					doc.Ascii[name] = ReadAscii(ms, offset_data, components);
				}
				else if (format == TagFormat::URational || format == TagFormat::SRational) {
					for (unsigned i = 0; i < components; ++i) {
						doc.Rational[name].push_back(ReadRational(ms, o, offset_data));
						offset_data += 8;
					}
				}
				else if (format == TagFormat::UInt32) {
					ms->Seek(offset_data, IO::SeekMethod::Begin);
					if (name == TagName::SubIfd || name == TagName::GpsIfd) {
						ReadIfd(ms, o, doc);
					}
					// Todo: Support UInt32 tags
				}
				else if (format == TagFormat::UInt16) {
					auto data = FirstUint16(offset_data, o);
					// Todo: Support components > 1
					doc.U16[name] = data;
				}
				ms->Seek(pos, IO::SeekMethod::Begin);
			}
		}

	}
}
