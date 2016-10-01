
#ifndef C_JPEG_H
#define C_JPEG_H

#include "codec_static.h"
#ifdef FAR
#undef FAR
#endif
extern "C" {
	#include <jpeglib.h>
}

#include <setjmp.h>

namespace Img {
	class CodecJPEG:public StaticCodec {
	public:
		CodecJPEG();
		~CodecJPEG();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		std::shared_ptr<Metadata::Document> PerformLoadMetadata() override;
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

		struct JpegError {
			struct jpeg_error_mgr pub;
			jmp_buf setjmp_buf;
			CodecJPEG* pCodec = nullptr;
		};

		struct SourceModule {
			jpeg_source_mgr pub;
			IO::FileReader::Ptr file;
			uint8_t buffer[BytesPerBuffer] = { 0 };
		};

		static boolean fill_input_buffer(j_decompress_ptr cinfo);
		static void skip_input_data(j_decompress_ptr cinfo, long num_bytes);
		static void term_source(j_decompress_ptr cinfo);
		static void init_source(j_decompress_ptr cinfo);

		SourceModule m_decompressionSource;

	private:
		struct jpeg_decompress_struct m_decInfo = {};
		bool m_isInit = false;

		std::string m_lastError;

		JSAMPROW m_pRow[ChunkRows];

		JpegError m_decErr;
		bool m_isError = false;

		IO::FileReader::Ptr m_file;
	};
}

#endif
