#include "c_bmp_rle.h"

namespace Img
{
	namespace Internal
	{
		BmpRleDecoder::BmpRleDecoder(std::shared_ptr<Img::Surface> destination, const BMPHeader& header):
			BmpDataDecoder{ destination, header },
			m_currPix{ 0 },
			m_row{ 0 }
		{
		
		}

		void BmpRleDecoder::AddPixelRLE(std::shared_ptr<Img::Surface::LockedArea> area, uint8_t val) {
			if (m_currPix >= m_header.Size.Width)
			{
				return;
			}

			area->Buffer()[m_currPix] = val;
			m_currPix++;
		}
	}
}
