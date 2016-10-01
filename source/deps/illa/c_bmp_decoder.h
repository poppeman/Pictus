#ifndef C_BMP_DECODER_H
#define C_BMP_DECODER_H

#include "bmp_header.h"
#include "surface.h"

namespace Img
{
	namespace Internal
	{
		class BmpDataDecoder
		{
		public:
			bool Process();

			BmpDataDecoder(std::shared_ptr<Img::Surface> destination, const BMPHeader& header);

		protected:
			std::shared_ptr<Img::Surface::LockedArea> GetScanlinePtr(int row);

			BMPHeader m_header;

		private:
			virtual bool OnProcess() = 0;

			std::shared_ptr<Img::Surface> m_destination;
		};
	}
}

#endif
