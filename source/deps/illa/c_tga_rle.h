#ifndef C_TGA_RLE_H
#define C_TGA_RLE_H

#include "orz/file_cache.h"
#include "c_tga_decoder.h"

namespace Img {
	class CodecTgaRleDecoder:public CodecTgaDecoder {
	private:
		enum {
			RowsPerBatch = 64
		};

		void OnSource();
		bool OnProcess(Batch* batch);

		template <typename T_Pixel, class T_Read>
		bool PerformOnProcess(Batch* batch, T_Read rd);

		IO::FileReaderByteStreamer m_fileStream;

		int m_numRlePixels;
		uint8_t m_repField;
		uint32_t m_pix;
		bool m_isRleBlock;
	};
}

#endif
