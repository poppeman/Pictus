#ifndef C_WBMP_H
#define C_WBMP_H

#include "codec_static.h"

namespace Img {
	class CodecWBMP:public StaticCodec {
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;
		Surface::Ptr PerformGetSurface();
		uint32_t ReadUIntVar(IO::FileReader::Ptr file);

		int m_currScan = 0;
		FileInt m_imageStart = 0;
	};
}

#endif
