#include "c_bmp_raw_24.h"
#include "surface_locked_area.h"

namespace Img
{
	namespace Internal
	{
		BmpRawDecoder24::BmpRawDecoder24(std::shared_ptr<Img::Surface> destination, BMPHeader header, std::shared_ptr<IO::FileReader> fileStream) :
			BmpDataDecoder{ destination, header },
			m_row{ 0 },
			m_reader{ fileStream }
		{

		}

		bool BmpRawDecoder24::OnProcess()
		{
			auto area = GetScanlinePtr(m_row++);
			auto pData32 = reinterpret_cast<uint32_t*>(area->Buffer());

			auto bytes = m_header.ColorDepth / 8;

			// Prettier to use 32-bit arrays for 32-bit buffers
			for (int currX = 0; currX < m_header.Size.Width; currX++) {
				uint32_t currentPixel = 0;
				uint8_t* pByteCurrentPixel = reinterpret_cast<uint8_t*>(&currentPixel);

				for (int b = 0; b < bytes; b++)
				{
					m_reader->ReadFull(&pByteCurrentPixel[b], 1);
				}

				// Convert pixel data and store in the surface
				*(pData32++) =
					0xff000000 +	// Alpha
					(((currentPixel & m_header.RedMask) >> m_header.RedShiftRight) << 16) +	// Red
					(((currentPixel & m_header.GreenMask) >> m_header.GreenShiftRight) << 8) +	// Green
					(((currentPixel & m_header.BlueMask) >> m_header.BlueShiftRight));			// Blue
			}
			m_reader->Seek(m_header.ScanPadding, IO::SeekMethod::Current);

			return m_row == m_header.Size.Height;
		}
	}
}
