#ifndef PSD_SHARED_H
#define PSD_SHARED_H

#include "orz/file.h"

namespace Img {
	namespace Internal {
		uint16_t ReadWordBE(IO::FileReader::Ptr file);
		uint32_t ReadDWordBE(IO::FileReader::Ptr file);
	}
}

#endif
