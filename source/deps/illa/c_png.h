#ifndef C_PNG_H
#define C_PNG_H

#include "codec_static.h"
#include "../libpng/png.h"

namespace Img {
	class CodecPNG:public StaticCodec {
	public:
		CodecPNG();
		~CodecPNG();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info);

		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		void destroy_png();

		static void libpng_error(png_structp png_ptr, png_const_charp message);
		static void libpng_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length);

		enum {
			ChunkRows = 8,
			SignBytes = 8,
		};

		Palette m_palette;
		int m_bitdepth;
		int m_chans;

		png_structp m_png_ptr;
		png_infop m_info_ptr;
		png_infop m_info_end_ptr;
		int m_passes;

		int m_currScan;
		int m_currPass;
	};
}

#endif
