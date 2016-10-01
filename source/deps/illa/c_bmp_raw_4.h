#ifndef ILLA_C_BMP_RAW_4_H
#define ILLA_C_BMP_RAW_4_H

#include "c_bmp_decoder.h"

namespace Img
{
	namespace Internal
	{
		class BmpRawDecoder4 :public BmpDataDecoder
		{
		public:
			BmpRawDecoder4(std::shared_ptr<Img::Surface> destination, const BMPHeader& header, std::shared_ptr<IO::FileReader> fileStream);

		private:
			bool OnProcess() override;

			std::shared_ptr<IO::FileReader> m_reader;
			int m_row;
		};
	}
}

#endif
