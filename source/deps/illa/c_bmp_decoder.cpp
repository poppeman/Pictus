#include "c_bmp_decoder.h"

namespace Img
{
	namespace Internal
	{
		using namespace Geom;

		bool BmpDataDecoder::Process()
		{
			return OnProcess();
		}

		BmpDataDecoder::BmpDataDecoder(std::shared_ptr<Img::Surface> destination, BMPHeader header):
			m_header{ header },
			m_destination{ destination }
		{

		}

		std::shared_ptr<Img::Surface::LockedArea> BmpDataDecoder::GetScanlinePtr(int row)
		{
			if (m_header.FlipVertical)
			{
				row = m_destination->Height() - row - 1;
			}
			return m_destination->LockSurface({ PointInt{0, row}, SizeInt{m_destination->Width(), 1} }, Img::LockReadWrite);
			auto area = m_destination->LockSurface({ PointInt{0, row}, SizeInt{m_destination->Width(), 1} }, Img::LockReadWrite);
			if(area == nullptr)
			{
				DO_THROW(Err::CriticalError, "Could not get scanline ptr");
			}
			return area;
		}
	}
}
