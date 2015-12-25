#ifndef ILLA_WEBP_IMAGECOMPOSER_H
#define ILLA_WEBP_IMAGECOMPOSER_H

#include "imagecomposer.h"

namespace Img {
	struct WebpFrame {
		int Delay;
		std::shared_ptr<Img::Surface> Surface;
	};

	class WebpImageComposer:public ::Img::ImageComposer {
	public:
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
	};
}

#endif
