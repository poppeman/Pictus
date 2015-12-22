#include "c_webp.h"
#include "orz/logger.h"
#include "illa/surface_locked_area.h"

namespace Img {
	static const FileInt HeaderSize = 30;
	static const FileInt ChunkSize = 1024;

	CodecWebp::CodecWebp():
		m_decoder(nullptr)
	{}
	CodecWebp::~CodecWebp() {
		if(m_decoder != nullptr)
		{
			WebPIDelete(m_decoder);
			m_decoder = nullptr;
		}
	}

	bool CodecWebp::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		if(WebPInitDecoderConfig(&m_config) == 0) {
			Log << "Failed initializing libwebp\n";
			return false;
		}
		Log << "Initialized libwebp\n";

		if(file->Size() < HeaderSize)
		{
			Log << "File too small, probably not valid\n";
			return false;
		}
		m_data.resize(HeaderSize);
		file->ReadFull(&m_data[0], HeaderSize);
		if(WebPGetFeatures(&m_data[0], m_data.size(), &m_config.input) != VP8_STATUS_OK) {
			Log << "Failed fetching WebP features, probably not a WebP image\n";
			return false;
		}

		info.Dimensions = {m_config.input.width, m_config.input.height};
		info.SurfaceFormat = m_config.input.has_alpha ? Img::Format::ARGB8888 : Img::Format::XRGB8888;

		return true;
	}

	AbstractCodec::AllocationStatus CodecWebp::PerformAllocate()
	{
		m_decoder = WebPIDecode(nullptr, 0, &m_config);

		GetSurface()->CreateSurface(GetSize(), GetFormat());
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecWebp::PerformLoadImageData(IO::FileReader::Ptr file)
	{
		m_config.output.colorspace = MODE_BGRA;
		m_config.output.is_external_memory = 1;

		while (DoTerminate() == false) {
			auto lock = GetSurface()->LockSurface();
			m_config.output.u.RGBA.rgba = lock->Buffer();
			m_config.output.u.RGBA.stride = lock->Stride();
			m_config.output.u.RGBA.size = lock->Size();

			if (file->Position() < file->Size()) {
				std::cout << "Reading " << ChunkSize << " at " << file->Position() << "\n";
				IO::ReadAppend(file, m_data, ChunkSize);
				auto status = WebPIUpdate(m_decoder, &m_data[0], m_data.size());
				WebPIDecGetRGB(m_decoder, nullptr, nullptr, nullptr, nullptr);
				if (status != VP8_STATUS_SUSPENDED)
				{
					break;
				}
			}
		}

		return LoadStatus::Aborted;
	}
}
