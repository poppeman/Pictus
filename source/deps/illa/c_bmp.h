#ifndef C_BMP_H
#define C_BMP_H

#include "orz/file_cache.h"
#include "codec_static.h"
#include "types.h"
#include "bmp_fileheader.h"
#include "bmp_header.h"

namespace Img {
	class Surface;

	class CodecBMP:public StaticCodec {
	public:
		CodecBMP();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		bool decompressRawRgb(int nRows, IO::FileReader::Ptr file, uint8_t* m_pData, ptrdiff_t &yofs, ptrdiff_t linestep);

		ptrdiff_t decompressRawIndex4_1(int nRows, IO::FileReader::Ptr file, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep);
		ptrdiff_t decompressRawIndex8(int nRows, IO::FileReader::Ptr file, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep);

		ptrdiff_t decompressRawRgb24_32(int nRows, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep);
		ptrdiff_t decompressRawRgb16(int nRows, uint8_t* m_pData, ptrdiff_t yofs, ptrdiff_t linestep);

		bool decompressRle8( Surface::LockedArea::Ptr area );
		bool decompressRle4( IO::FileReader::Ptr file, Surface::LockedArea::Ptr area );

	private:
		enum {
			ChunkRows = 64,
			RLEChunks = 64,
			RLE_EOL = 0,	// End Of Line
			RLE_EOB = 1,	// End Of Bitmap
			RLE_DELTA = 2,	// Apply delta to position
		};

	private:
		void DecodeRLE();
		void AddPixelRLE(Surface::LockedArea::Ptr area, uint8_t val);

	private:
		IO::FileReaderByteStreamer m_fileStream;

		// Bitmap data
		Internal::BMPFileHeader m_bfh;
		Internal::BMPHeader m_header;

		// Loading state
		int m_currScan;	// For RGB and RLE
		int m_currPix;	// For RLE
		bool m_isValid;
	};
}

#endif
