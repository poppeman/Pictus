#include "c_bmp_raw_16.h"
#include "orz/orz_math.h"
#include "surface_locked_area.h"

namespace Img
{
	namespace Internal
	{
		BmpRawDecoder16::BmpRawDecoder16(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream):
			BmpDataDecoder{ destination, header },
			m_reader{ fileStream },
			m_row{ 0 }
		{

		}

		bool BmpRawDecoder16::OnProcess()
		{
			auto area = GetScanlinePtr(m_row++);
			auto pData8 = area->Buffer();

			if (Math::Bit::CountSet(m_header.GreenMask) == 6) {
				// No need to set alpha bit.
				for (int currX = 0; currX < m_header.Size.Width; ++currX)
				{
					m_reader->ReadFull(pData8++, 1);
					m_reader->ReadFull(pData8++, 1);
				}
			}
			else {
				// Set alpha bit.
				for (int currX = 0; currX < m_header.Size.Width; ++currX)
				{
					m_reader->ReadFull(pData8++, 1);
					uint8_t b2;
					m_reader->ReadFull(&b2, 1);
					*pData8++ = b2 | 0x80;
				}
			}

			m_reader->Seek(m_header.ScanPadding, IO::SeekMethod::Current);

			return m_row == m_header.Size.Height;
		}
	}
}
