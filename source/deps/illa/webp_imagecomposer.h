#ifndef ILLA_WEBP_IMAGECOMPOSER_H
#define ILLA_WEBP_IMAGECOMPOSER_H

#include "imagecomposer.h"

namespace Img {
	enum class WebpDispose {
		None,
		BackgroundColor
	};

	enum class WebpBlendMethod {
		None,
		Alpha
	};

	struct WebpFrame {
		int Delay;
		std::shared_ptr<Img::Surface> Surface;
		WebpDispose DisposeMethod;
		WebpBlendMethod BlendMethod;
		Geom::PointInt Offset;
	};

	class WebpImageComposer:public ::Img::ImageComposer {
	public:
		void SetCanvasSize(Geom::SizeInt newSize);
		void SetBackgroundColor(Img::Color backgroundColor);
		void SetFrameCount(size_t numFrames);
		void SendFrame(WebpFrame frame);

		WebpImageComposer();
		~WebpImageComposer();

	private:
		Surface::Ptr ComposeCurrentSurface() override;
		void OnAdvance() override;
		int OnDelay() override;
		void OnRestart() override;

		std::vector<WebpFrame> m_frames;
		std::mutex m_mutFrames;
		size_t m_currFrame;
		size_t m_numFrames;
		std::shared_ptr<Img::Surface> m_currentSurface;
		Geom::SizeInt m_dims;
		Img::Color m_backgroundColor;
	};
}

#endif
