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

			enum class Layout {
				Undefined,
				Pix565,
				Pix555,
				Pix444
			};

			int m_row;
			Layout m_layout;
			std::shared_ptr<IO::FileReader> m_reader;
		};
	}
}

#endif
