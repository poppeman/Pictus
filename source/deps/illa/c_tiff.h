#ifndef C_TIFF_H
#define C_TIFF_H

#include "codec_static.h"
#include "types.h"

#include "tiff_loader.h"

namespace Img {
	class CodecTIFF:public StaticCodec {
	public:
		~CodecTIFF();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

	private:
		void endImageIO();

	private:
		TIFF* m_tiff = nullptr;
		uint16_t m_photometric = 0;

		TiffLoader::Ptr m_loader;
		TiffDataConverter::Ptr m_converter;

		Img::Palette m_palette;
	};
}

#endif
