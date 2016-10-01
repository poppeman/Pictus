#include "c_webp.h"
#include "orz/logger.h"
#include "illa/surface_locked_area.h"
#include "surfacemgr.h"
#include <type_traits>

namespace Img {
	static const FileInt HeaderSize = 12;
	static const FileInt ChunkSize = 1024;

	ImageComposer::Ptr CodecWebp::RequestImageComposer() {
		return m_composer;
	}

	CodecWebp::CodecWebp():
		m_composer(std::make_shared<Img::WebpImageComposer>())
	{}

	CodecWebp::~CodecWebp() {
		if(m_decoder != nullptr)
		{
			WebPIDelete(m_decoder);
			m_decoder = nullptr;
		}
	}

	bool CodecWebp::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		static_assert(HeaderSize >= 12, "HeaderSize must be at least 12");

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
		if(strncmp(reinterpret_cast<char*>(&m_data[0]), "RIFF", 4) || strncmp(reinterpret_cast<char*>(&m_data[8]), "WEBP", 4))
		{
			return false;
		}

		// Most things in the API expects that all data is readily available in memory.
		IO::ReadAppend(file, m_data);
		WebPDataInit(&m_wpdata);
		m_wpdata.bytes = &m_data[0];
		m_wpdata.size = m_data.size();

		m_mux = WebPDemux(&m_wpdata);
		info.Dimensions = Geom::SizeInt
		{
			static_cast<int>(WebPDemuxGetI(m_mux, WEBP_FF_CANVAS_WIDTH)),
			static_cast<int>(WebPDemuxGetI(m_mux, WEBP_FF_CANVAS_HEIGHT))
		};
		auto flags = WebPDemuxGetI(m_mux, WEBP_FF_FORMAT_FLAGS);

		info.SurfaceFormat = (flags & ALPHA_FLAG) ? Img::Format::ARGB8888 : Img::Format::XRGB8888;

		if(WebPGetFeatures(&m_data[0], m_data.size(), &m_config.input) != VP8_STATUS_OK)
		{
			return false;
		}

		m_numFrames = WebPDemuxGetI(m_mux, WEBP_FF_FRAME_COUNT);

		info.Dimensions = {m_config.input.width, m_config.input.height};
		m_composer->SetCanvasSize(info.Dimensions);
		m_composer->SetBackgroundColor(Img::Color::FromDWord(WebPDemuxGetI(m_mux, WEBP_FF_BACKGROUND_COLOR)));
		m_composer->SetFrameCount(m_numFrames);
		return true;
	}

	AbstractCodec::AllocationStatus CodecWebp::PerformAllocate()
	{
		for(auto i=0u; i < m_numFrames; i++)
		{
			WebPIterator wpIter;
			WebPDemuxGetFrame(m_mux, i + 1, &wpIter);
			auto currSurface = CreateNewSurface();
			currSurface->CreateSurface({ wpIter.width, wpIter.height }, wpIter.has_alpha ? Img::Format::ARGB8888 : Img::Format::XRGB8888);
			m_frames.push_back({
				wpIter.duration,
				currSurface,
				wpIter.dispose_method == WEBP_MUX_DISPOSE_BACKGROUND ? WebpDispose::BackgroundColor : WebpDispose::None,
				wpIter.blend_method == WEBP_MUX_BLEND ? WebpBlendMethod::Alpha : WebpBlendMethod::None,
				{ wpIter.x_offset, wpIter.y_offset }});

			WebPDemuxReleaseIterator(&wpIter);
		}
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecWebp::PerformLoadImageData(IO::FileReader::Ptr file)
	{
		m_config.output.colorspace = MODE_BGRA;
		m_config.output.is_external_memory = 1;

		while(DoTerminate() == false)
		{
			if(m_currFrame >= m_numFrames)
			{
				return AbstractCodec::LoadStatus::Finished;
			}

			WebPIterator wpIter;
			if (WebPDemuxGetFrame(m_mux, m_currFrame + 1, &wpIter) == 0)
			{
				Log << "Failed getting frame\n";
				return LoadStatus::Failed;
			}

			{
				auto lock = m_frames[m_currFrame].Surface->LockSurface();
				m_config.output.u.RGBA.rgba = lock->Buffer();
				m_config.output.u.RGBA.stride = lock->Stride();
				m_config.output.u.RGBA.size = lock->Size();

				WebPDecode(wpIter.fragment.bytes, wpIter.fragment.size, &m_config);
			}
			WebPDemuxReleaseIterator(&wpIter);

			m_composer->SendFrame(m_frames[m_currFrame++]);
		}

		return LoadStatus::Aborted;
	}
	size_t CodecWebp::PerformEstimateMemory()
	{
		return GetSize().Width * GetSize().Height * 4 * m_numFrames;
	}
}
