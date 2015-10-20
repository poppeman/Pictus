#include "c_bmp_raw_4.h"
#include "orz/bitconvert.h"
#include "surface_locked_area.h"

namespace Img
{
	namespace Internal
	{
		BmpRawDecoder4::BmpRawDecoder4(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream):
			BmpDataDecoder{ destination, header },
			m_reader{ fileStream },
			m_row{ 0 }
		{

		}

		bool BmpRawDecoder4::OnProcess()
		{
			int pixelsPerByte = 8 / m_header.ColorDepth;
			int totalBytesPerRow = (m_header.Size.Width + pixelsPerByte - 1) / pixelsPerByte;

			auto area = GetScanlinePtr(m_row++);
			auto currScanSource = std::make_unique<uint8_t[]>(totalBytesPerRow);

			m_reader->ReadFull(currScanSource.get(), totalBytesPerRow);

			if (m_header.ColorDepth == 1)
			{
				Util::BitsToBytes(area->Buffer(), currScanSource.get(), m_header.Size.Width);
			}
			else if (m_header.ColorDepth == 4)
			{
				Util::NibblesToBytes(area->Buffer(), currScanSource.get(), m_header.Size.Width);
			}

			m_reader->Seek(m_header.ScanPadding, IO::SeekMethod::Current);

			return m_row == m_header.Size.Height;
		}
	}
}
