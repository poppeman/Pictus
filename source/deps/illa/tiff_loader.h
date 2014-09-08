#ifndef TIFF_LOADER_H
#define TIFF_LOADER_H

#include "surface.h"
#include "tiff_converter.h"
#include "types.h"

namespace Img {
	enum class TiffChunkStatus {
		ChunksRemaining,
		Finished,
		Error
	};

	class TiffLoader {
	public:
		void Create(TIFF* tiff, Surface::Ptr surface, TiffDataConverter::Ptr converter);
		TiffChunkStatus LoadChunk();

		virtual ~TiffLoader()=0;

		typedef std::shared_ptr<TiffLoader> Ptr;

	protected:
		TIFF* m_tiff;
		TiffDataConverter::Ptr m_converter;
		Surface::Ptr m_surface;

	private:
		virtual void PerformOnCreate()=0;
		virtual TiffChunkStatus PerformLoadChunk() = 0;
	};
}

#endif
