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
			m_destination{ destination },
			m_header{ header }
		{

		}

		std::shared_ptr<Img::Surface::LockedArea> BmpDataDecoder::GetScanlinePtr(int row)
		{
			if (m_header.FlipVertical)
			{
				row = m_destination->Height() - row - 1;
			}
			return m_destination->LockSurface({ PointInt{0, row}, SizeInt{m_destination->Width(), 1} }, Img::LockReadWrite);
		}
	}
}
