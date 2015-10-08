#include "Exif_Converter.h"
#include "Exif_IO.h"
#include "Exif_Parser.h"
#include "Exif_ReadIfd.h"
#include "Exif_Types.h"
#include "../orz/file_reader.h"
#include "../orz/stream_mem.h"

namespace Metadata {
	namespace Exif {
		std::shared_ptr<Document> Decode(unsigned char* data, size_t length) {
			auto doc = std::shared_ptr<Document>();

			auto ms = std::make_shared<IO::FileReader>(std::make_shared<IO::StreamMemory>(data, length));
			ms->Open();

			ByteOrder bo = ByteOrder::Undefined;
			{
				uint16_t order;
				ms->ReadFull(&order, 2);

				if (order == 0x4949) {
					bo = ByteOrder::Intel;
				}

				if (order == 0x4d4d) {
					bo = ByteOrder::Motorola;
				}
			}
			if (bo == ByteOrder::Undefined) {
				return doc;
			}

			if (ReadUint16(ms, bo) != 0x2a) {
				return doc;
			}

			auto ifd_offset = ReadUint32(ms, bo);
			ms->Seek(ifd_offset, IO::SeekMethod::Begin);

			doc.reset(new Document);
			ExifDocument tc;
			ReadIfd(ms, bo, tc);
			Converter c(tc, doc);
			c.Flatten();

			return doc;
		}
	}
}
