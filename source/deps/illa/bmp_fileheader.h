#ifndef BMP_FILEHEADER_H
#define BMP_FILEHEADER_H

#include "orz/file_reader.h"

namespace Img {
	namespace Internal {
		class BMPFileHeader {
		public:
			void Load(IO::FileReader::Ptr file);
			BMPFileHeader();

			uint16_t type = 0;
			uint32_t size = 0;
			uint32_t offset = 0;
		};
	}
}

#endif
