#include "c_bmp_rle_4.h"

namespace Img
{
	namespace Internal
	{
		BmpRleDecoder4::BmpRleDecoder4(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream) :
			BmpRleDecoder{ destination, header },
			m_reader{ fileStream }
		{
		
		}

		bool BmpRleDecoder4::OnProcess()
		{
			std::shared_ptr<Img::Surface::LockedArea> area = GetScanlinePtr(m_row);

			for (int chunk = 0; chunk < RLEChunks; ++chunk) {
				uint8_t b1, b2;
				m_reader->ReadFull(&b1, 1);
				m_reader->ReadFull(&b2, 1);

				if (b1 == 0)
				{
					// Special mode
					switch (b2) {
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
						int numBytes = b2 >> 1;
						for (auto i = 0; i < numBytes; ++i)
						{
							uint8_t val;
							m_reader->ReadFull(&val, 1);

							AddPixelRLE(area, (val & 0xf0) >> 4);
							AddPixelRLE(area, (val & 0x0f));
						}
						if (b2 & 0x1)
						{
							uint8_t val;
							m_reader->ReadFull(&val, 1);
							AddPixelRLE(area, (val & 0xf0) >> 4);
						}

						if ((numBytes + (b2 & 0x01)) & 0x1)
						{
							m_reader->Seek(1, IO::SeekMethod::Current);
						}
						break;
					}
				}
				else
				{
					int numBytes = b1 >> 1;
					uint8_t pix1 = (b2 & 0xf0) >> 4;
					uint8_t pix2 = b2 & 0xf;

					for (uint8_t i = 0; i < numBytes; ++i)
					{
						AddPixelRLE(area, pix1);
						AddPixelRLE(area, pix2);
					}

					if (b1 & 0x01) {
						AddPixelRLE(area, pix1);
					}
				}
			}
			return m_row == m_header.Size.Height;
		}
	}
}
