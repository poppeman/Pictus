#include "c_bmp_rle_8.h"

namespace Img
{
	namespace Internal
	{
		BmpRleDecoder8::BmpRleDecoder8(std::shared_ptr<Img::Surface> destination, const BMPHeader& header, std::shared_ptr<IO::FileReader> fileStream):
			BmpRleDecoder{ destination, header },
			m_reader{ fileStream }
		{
		
		}

		bool BmpRleDecoder8::OnProcess()
		{
			std::shared_ptr<Img::Surface::LockedArea> area = GetScanlinePtr(m_row);

			for (int chunk = 0; chunk < RLEChunks; ++chunk) {
				uint8_t b1, b2;
				m_reader->ReadFull(&b1, 1);
				m_reader->ReadFull(&b2, 1);

				if (b1 == 0)
				{
					// Special mode
					switch (b2)
					{
					case RLE_EOL:		// Switch to next line
						if (m_currPix != 0)
						{
							m_row++;
							m_currPix = 0;
							area = nullptr;
							area = GetScanlinePtr(m_row);
						}
						break;
					case RLE_EOB:		// Done!
						return true;
					case RLE_DELTA:		// Change position
					{
						uint8_t dx, dy;
						m_reader->ReadFull(&dx, 1);
						m_reader->ReadFull(&dy, 1);
						m_currPix += dx;
						m_row += dy;
						area = nullptr;
						area = GetScanlinePtr(m_row);
					}
						break;
					default:			// Absolute mode run
						for (uint8_t i = 0; i < b2; ++i)
						{
							uint8_t val;
							m_reader->ReadFull(&val, 1);
							AddPixelRLE(area, val);
						}

						if (b2 & 0x1)
						{
							m_reader->Seek(1, IO::SeekMethod::Current);
						}
						break;
					}
				}
				else
				{
					for (uint8_t i = 0; i < b1; ++i)
					{
						AddPixelRLE(area, b2);
					}
				}
			}
			return m_row == m_header.Size.Height;
		}
	}
}
