#ifndef C_TGA_DECODER_H
#define C_TGA_DECODER_H

#include "orz/file.h"
#include "surface.h"
#include "c_tga_header.h"
#include "orz/file_cache.h"

namespace Img {
	class CodecTgaDecoder {
	public:
		bool Process();
		void SetSource(IO::FileReader::Ptr reader, const TGAHeader* header);
		void SetDestination(Img::Surface::Ptr surfaceToProcess);

		typedef std::shared_ptr<CodecTgaDecoder> Ptr;

	protected:
		IO::FileReader::Ptr m_reader;
		Img::Surface::Ptr m_surface;
		const TGAHeader* m_header;

		struct Batch {
			int StartScan;
			int NumRows;
			ptrdiff_t SignedScanlineStride;
			int UnsignedPixelStep;
			Surface::LockedArea::Ptr Area;
			uint8_t* Destination;
		};

		template <typename T> void FlipHorizontalBatch(CodecTgaDecoder::Batch* batchToFlip) {
			int width = m_header->Size.Width;

			uint8_t* destination = batchToFlip->Destination;
			for (int y = 0; y < batchToFlip->NumRows; ++y) {
				T* dwArray1 = reinterpret_cast<T*>(destination);
				T* dwArray2 = reinterpret_cast<T*>(destination + sizeof(T) * (width - 1));

				for (int x = 0; x < (width / 2); ++x)
					std::swap(*(dwArray1++), *(dwArray2--));

				destination += batchToFlip->SignedScanlineStride;
			}
		}

	private:
		enum {
			RowsPerBatch = 64
		};

		virtual bool OnProcess(Batch* batch)=0;
		virtual void OnSource();

		Batch PrepareBatch( int rowsToProcess );
		bool FinalizeBatch(Batch* batch);

		int m_currentScanline;
	};
}

#endif
