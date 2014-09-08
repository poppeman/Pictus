#ifndef TIFF_CONVERTER_H
#define TIFF_CONVERTER_H

#include "types.h"

namespace Img {
	class TiffDataConverter {
	public:
		void Create(TIFF* tiff, Format dataFormat);

		uint8_t* RequestDestinationBuffer(size_t bytes);
		uint8_t* ReleaseDestinationBuffer(int widthInPixels, size_t bytesUsed, size_t* expandedSize);

		virtual ~TiffDataConverter();

		typedef std::shared_ptr<TiffDataConverter> Ptr;

	protected:
		TIFF* m_tiff;
		Format m_format;

	private:
		virtual void PerformCreate();
		virtual uint8_t* PerformRequestDestinationBuffer(size_t bytes)=0;
		virtual uint8_t* PerformReleaseDestinationBuffer(int widthInPixels, size_t bytesUsed, size_t* expandedSize)=0;
	};
}

#endif
