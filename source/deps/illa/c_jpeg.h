
#ifndef C_JPEG_H
#define C_JPEG_H

#include "codec_static.h"
#ifdef FAR
#undef FAR
#endif
extern "C" {
	#include "libjpeg/jpeglib.h"
};

#include "orz/file.h"

namespace Img {
	class CodecJPEG:public StaticCodec {
	public:
		CodecJPEG();
		~CodecJPEG();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		void PrepareLibJpeg( IO::FileReader::Ptr file );
		AllocationStatus PerformAllocate(const Geom::SizeInt& dimHint) override;
		AllocationStatus PerformAllocate() override;

		AllocationStatus CreateSurface();

		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

	private:
		enum {
			ChunkRows = 64,
			BytesPerBuffer = 1024
		};

	private:
		void destroy();

	private:
		static void jpeg_error_exit(j_common_ptr cinfo);

		struct SourceModule {
			jpeg_source_mgr pub;
			IO::FileReader::Ptr file;
			uint8_t buffer[BytesPerBuffer];
		};

		static boolean fill_input_buffer(j_decompress_ptr cinfo);
		static void skip_input_data(j_decompress_ptr cinfo, long num_bytes);
		static void term_source(j_decompress_ptr cinfo);
		static void init_source(j_decompress_ptr cinfo);

		SourceModule m_decompressionSource;

	private:
		struct jpeg_decompress_struct	m_decInfo;
		bool m_isInit;

		JSAMPROW m_pRow[ChunkRows];

		// Error handling 
		struct our_error {
			jpeg_error_mgr pub;
			CodecJPEG* pCodec;
		};

		our_error m_decErr;
		bool m_isError;

		IO::FileReader::Ptr m_file;
	};
}

#endif
