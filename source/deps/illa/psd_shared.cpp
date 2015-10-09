#include "psd_shared.h"

namespace Img {
	namespace Internal {
		uint16_t ReadWordBE(IO::FileReader::Ptr file) {
			uint8_t data[2];
			file->ReadFull(data, 2);
			return (data[0] << 8) + data[1];
		}

		uint32_t ReadDWordBE(IO::FileReader::Ptr file) {
			uint8_t data[4];
			file->ReadFull(data, 4);
			return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
		}
	}
}
