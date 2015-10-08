#ifndef METADATA_EXIF_IO_H
#define METADATA_EXIF_IO_H

#include <cstdint>
#include "../orz/file_reader.h"
#include "Exif_Types.h"

namespace Metadata {
	namespace Exif {
		uint16_t ReadUint16(IO::FileReader::Ptr ms, ByteOrder o, size_t position);
		uint16_t ReadUint16(IO::FileReader::Ptr ms, ByteOrder o);
		uint32_t ReadUint32(IO::FileReader::Ptr ms, ByteOrder o);
		uint16_t FirstUint16(uint32_t data, ByteOrder o);
		uint16_t SecondUint16(uint32_t data, ByteOrder o);
		std::string ReadAscii(IO::FileReader::Ptr ms, size_t offset, size_t components);
		Metadata::Rational ReadRational(IO::FileReader::Ptr ms, ByteOrder o, size_t position);
	}
}

#endif
