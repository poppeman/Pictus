#ifndef C_XYZ_H
#define C_XYZ_H

#include "codec_static.h"
#include "zlib/zlib.h"

#include <deque>

namespace Img {
	class CodecXYZ:public StaticCodec {
	public:
		CodecXYZ();
		~CodecXYZ();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

	private:
		bool FillBuffer(IO::FileReader::Ptr file);

		void Cleanup();
		enum {
			InBufferSize = 256,
			OutBufferSize = 1024
		};

		Geom::SizeInt m_size;
		bool m_paletteSet;

		z_stream m_stream;

		uint8_t m_inBuffer[InBufferSize];

		typedef std::deque<uint8_t> ByteStream;
		ByteStream m_outData;
		int m_x, m_y;
		bool m_isInit;

		uint8_t m_in_buffer[InBufferSize];
		uint8_t m_out_buffer[OutBufferSize];
	};
}

#endif
