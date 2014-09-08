#ifndef C_PCX_H
#define C_PCX_H

#include "orz/file_cache.h"
#include "codec_static.h"
#include "pcx_header.h"

namespace Img {
	class Surface;

	class CodecPCX:public StaticCodec {
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		Img::Format determineSurfaceFormat(uint8_t colorDepth);
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		void transferNonPlanar(uint8_t* destination, uint8_t* source);
		void transferPlanar(uint8_t* destination, uint8_t* source);

		void transferPlanar4(uint8_t* destination, uint8_t* source);
		void transferPlanar38(uint8_t* destination, uint8_t* source);

		enum {
			ChunkRows = 64,
		};

		void DecodeScanline(IO::FileReader::Ptr file, uint8_t* pDest, size_t maxBytes);

		Internal::PCXHeader m_header;
		int m_currScan;
		IO::FileReaderByteStreamer m_cache;
	};
}

#endif
