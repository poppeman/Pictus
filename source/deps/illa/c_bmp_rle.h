#ifndef ILLA_C_BMP_RLE_H
#define ILLA_C_BMP_RLE_H

#include "orz/file_cache.h"
#include "c_bmp_decoder.h"
#include "surface_locked_area.h"

namespace Img
{
	namespace Internal
	{
		class BmpRleDecoder :public BmpDataDecoder {
		public:
			BmpRleDecoder(std::shared_ptr<Img::Surface> destination, BMPHeader header);

		protected:
			enum {
				ChunkRows = 64,
				RLEChunks = 64,
				RLE_EOL = 0,	// End Of Line
				RLE_EOB = 1,	// End Of Bitmap
				RLE_DELTA = 2,	// Apply delta to position
			};

			virtual bool OnProcess()=0;

			void AddPixelRLE(std::shared_ptr<Img::Surface::LockedArea> area, uint8_t val);
			int m_currPix, m_row;
		};
	}
}

#endif

