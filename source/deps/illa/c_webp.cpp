#include "c_webp.h"
#include "orz/logger.h"

namespace Img {
	CodecWebp::CodecWebp() {}
	CodecWebp::~CodecWebp() {}

	bool CodecWebp::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		if(WebPInitDecoderConfig(&m_config) == 0) {
			Log << "Failed initializing libwebp\n";
			return false;
		}
		Log << "Initialized libwebp\n";

		auto size = file->Size();
		m_data.resize(size);
		file->ReadFull(&m_data[0], size);

		if(WebPGetFeatures(&m_data[0], m_data.size(), &m_config.input) != VP8_STATUS_OK) {
			Log << "Failed fetching WebP features, probably not a WebP image\n";
			return false;
		}


		info.SurfaceFormat = Img::Format::XRGB8888;
		info.Dimensions = {m_config.input.width, m_config.input.height};

		return true;
	}

	AbstractCodec::AllocationStatus CodecWebp::PerformAllocate()
	{
		return AllocationStatus::Failed;
	}


	AbstractCodec::LoadStatus CodecWebp::PerformLoadImageData(IO::FileReader::Ptr file)
	{
		return AbstractCodec::LoadStatus::Failed;
	}
}