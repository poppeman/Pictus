#ifndef C_TGA_H
#define C_TGA_H

#include "codec_static.h"
#include "c_tga_decoder.h"

namespace Img {
	class CodecTGA:public StaticCodec {
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		CodecTgaDecoder::Ptr m_decoder;

		// Image data
		TGAHeader m_header;
		Palette m_palette;
	};
}

#endif
