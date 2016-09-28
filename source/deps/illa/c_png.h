#ifndef C_PNG_H
#define C_PNG_H

#include "codec_static.h"
#include <png.h>
#include <setjmp.h>

namespace Img {
	class CodecPNG:public StaticCodec {
	public:
		~CodecPNG();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		void destroy_png();

		static void libpng_error(png_structp png_ptr, png_const_charp message);
		static void libpng_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length);

		enum {
			ChunkRows = 8,
			SignBytes = 8,
		};

		jmp_buf m_setjmpBuf;
		std::string m_lastError;

		Palette m_palette;
		int m_bitdepth = 0;
		int m_chans = 0;

		png_structp m_png_ptr;
		png_infop m_info_ptr;
		int m_passes = 0;

		int m_currScan = 0;
		int m_currPass = 0;
	};
}

#endif
