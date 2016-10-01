#include "c_bmp_raw_8.h"
#include "illa/surface_locked_area.h"

namespace Img
{
	namespace Internal
	{

		BmpRawDecoder8::BmpRawDecoder8(std::shared_ptr<Img::Surface> destination, const BMPHeader& header, std::shared_ptr<IO::FileReader> fileStream) :
			BmpDataDecoder{ destination, header },
			m_reader{ fileStream },
			m_row{ 0 }
		{

		}

		bool BmpRawDecoder8::OnProcess()
		{
			auto bytes = m_header.Size.Width * m_header.ColorDepth / 8;
			auto area = GetScanlinePtr(m_row++);
			m_reader->ReadFull(area->Buffer(), bytes);
			m_reader->Seek(m_header.ScanPadding, IO::SeekMethod::Current);
			return m_row == m_header.Size.Height;
		}
	}
}
