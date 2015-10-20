#ifndef ILLA_C_BMP_RAW_8_H
#define ILLA_C_BMP_RAW_8_H

#include "bmp_header.h"
#include "c_bmp_decoder.h"

#include "orz/file_reader.h"

namespace Img
{
	namespace Internal
	{
		class BmpRawDecoder8 :public BmpDataDecoder
		{
		public:
			BmpRawDecoder8(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream);

		private:
			bool OnProcess() override;

			std::shared_ptr<IO::FileReader> m_reader;
			int m_row;
		};
	}
}

#endif

