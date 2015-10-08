#ifndef METADATA_EXIF_READIFD_H
#define METADATA_EXIF_READIFD_H

#include "../orz/file_reader.h"
#include "Exif_Types.h"

namespace Metadata {
	namespace Exif {
		void ReadIfd(IO::FileReader::Ptr ms, ByteOrder o, ExifDocument& doc);
	}
}

#endif
