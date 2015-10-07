#ifndef IMAGE_EXIF_PARSER_H
#define IMAGE_EXIF_PARSER_H

#include "Metadata_Document.h"

namespace Metadata {
	namespace Exif {
		std::shared_ptr<Document> Decode(unsigned char* data, size_t length);
	}
}

#endif
