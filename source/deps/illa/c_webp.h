#ifndef C_WEBP_H
#define C_WEBP_H

#include "codec_static.h"
#include "../libwebp/src/webp/decode.h"

namespace Img {
	class CodecWebp:public StaticCodec {
	public:
		CodecWebp();
		~CodecWebp();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		WebPDecoderConfig m_config;
		std::vector<uint8_t> m_data;
	};
}

#endif
