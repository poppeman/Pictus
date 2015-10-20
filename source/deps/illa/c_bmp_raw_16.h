#ifndef ILLA_C_BMP_RAW_16_H
#define ILLA_C_BMP_RAW_16_H

#include "c_bmp_decoder.h"

namespace Img
{
	namespace Internal
	{
		class BmpRawDecoder16 :public BmpDataDecoder
		{
		public:
			explicit BmpRawDecoder16(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream);

		private:
			bool OnProcess() override;

			int m_row;
			std::shared_ptr<IO::FileReader> m_reader;
		};
	}
}

#endif
