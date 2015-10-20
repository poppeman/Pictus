#ifndef ILLA_C_BMP_RAW_24_H
#define ILLA_C_BMP_RAW_24_H

#include "orz/file_cache.h"
#include "c_bmp_decoder.h"

namespace Img
{
	namespace Internal
	{
		class BmpRawDecoder24 :public BmpDataDecoder
		{
		public:
			explicit BmpRawDecoder24(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream);

		private:
			bool OnProcess() override;

			int m_row;
			std::shared_ptr<IO::FileReader> m_reader;
		};
	}
}

#endif
