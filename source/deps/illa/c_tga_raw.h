#ifndef C_TGA_RAW_H
#define C_TGA_RAW_H

#include "orz/file_cache.h"
#include "c_tga_decoder.h"

namespace Img {
	class CodecTgaRawDecoder:public CodecTgaDecoder {
		void OnSource();
		bool OnProcess(Batch* batch);

		IO::FileReaderByteStreamer m_fileStream;
	};
}

#endif
