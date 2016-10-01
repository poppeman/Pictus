#ifndef ILLA_C_WEBP_H
#define ILLA_C_WEBP_H

#include "codec_static.h"
#include "webp_imagecomposer.h"
#include <webp/decode.h>
#include <webp/demux.h>

namespace Img {
	class CodecWebp:public AbstractCodec {
	public:
		ImageComposer::Ptr RequestImageComposer() override;

		CodecWebp();
		~CodecWebp();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;
		size_t PerformEstimateMemory() override;

		std::vector<WebpFrame> m_frames;
		std::shared_ptr<WebpImageComposer> m_composer;

		WebPDecoderConfig m_config = {};
		WebPIDecoder* m_decoder = nullptr;
		WebPDemuxer* m_mux = nullptr;
		WebPData m_wpdata = {};
		std::vector<uint8_t> m_data;
		size_t m_currFrame = 0;
		size_t m_numFrames = 0;
	};
}

#endif
