#ifndef BMP_FILEHEADER_H
#define BMP_FILEHEADER_H

#include "orz/file.h"

namespace Img {
	namespace Internal {
		class BMPFileHeader {
		public:
			void Load(IO::FileReader::Ptr file);
			BMPFileHeader();

			uint16_t type;
			uint32_t size;
			uint32_t offset;
		};
	}
}

#endif
