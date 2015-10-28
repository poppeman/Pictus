#include "c_bmp_raw_16.h"
#include "orz/orz_math.h"
#include "surface_locked_area.h"

namespace Img
{
	namespace Internal
	{
		BmpRawDecoder16::BmpRawDecoder16(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream):
			BmpDataDecoder{ destination, header },
			m_row{ 0 },
			m_layout{ Layout::Undefined },
			m_reader{ fileStream }
		{
			const int blueBits = Math::Bit::CountSet(m_header.BlueMask);
			const int greenBits = Math::Bit::CountSet(m_header.GreenMask);
			const int redBits = Math::Bit::CountSet(m_header.RedMask);

			if (redBits == 5 && greenBits == 6 && blueBits == 5)
			{
				m_layout = Layout::Pix565;
			}
			else if (redBits == 5 && greenBits == 5 && blueBits == 5)
			{
				m_layout = Layout::Pix555;
			}
			else if (redBits == 8 && greenBits == 8 && blueBits == 8)
			{
				// There are files that claim to be 16 bit images with 8 bit channels, but these are likely to
				// be 1555 images in reality.
				m_layout = Layout::Pix555;
			}
			if (m_layout == Layout::Undefined)
			{
				DO_THROW(Err::Unsupported, "Pixel layout not recognized: " + ToAString(m_header.RedMask) + ToAString(m_header.GreenMask) + ToAString(m_header.BlueMask));
			}
		}

		bool BmpRawDecoder16::OnProcess()
		{
			auto area = GetScanlinePtr(m_row++);
			auto pData8 = area->Buffer();

			switch (m_layout)
			{
			case Img::Internal::BmpRawDecoder16::Layout::Pix565:
				// No need to set alpha bit.
				for (int currX = 0; currX < m_header.Size.Width; ++currX)
				{
					m_reader->ReadFull(pData8++, 1);
					m_reader->ReadFull(pData8++, 1);
				}
				break;

			case Img::Internal::BmpRawDecoder16::Layout::Pix555:
				for (int currX = 0; currX < m_header.Size.Width; ++currX)
				{
					m_reader->ReadFull(pData8++, 1);
					uint8_t b2;
					m_reader->ReadFull(&b2, 1);
					*pData8++ = b2 | 0x80;
				}
				break;
			default:
				DO_THROW(Err::CriticalError, u8"Encountered an unsupported pixel layout in OnProcess, this is a decoder bug");
			}

			m_reader->Seek(m_header.ScanPadding, IO::SeekMethod::Current);

			return m_row == m_header.Size.Height;
		}
	}
}
