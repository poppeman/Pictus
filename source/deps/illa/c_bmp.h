#ifndef C_BMP_H
#define C_BMP_H

#include "orz/file_cache.h"
#include "codec_static.h"
#include "types.h"
#include "bmp_fileheader.h"
#include "bmp_header.h"
#include "c_bmp_decoder.h"

#include <cstddef>

namespace Img {
	class Surface;

	class CodecBMP:public StaticCodec {
	public:
		CodecBMP();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

	private:
		IO::FileReaderByteStreamer m_fileStream;

		// Bitmap data
		Internal::BMPFileHeader m_bfh;
		Internal::BMPHeader m_header;

		std::shared_ptr<Internal::BmpDataDecoder> m_dec;
	};
}

#endif
