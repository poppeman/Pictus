#include "webp_imagecomposer.h"

namespace Img {
	void WebpImageComposer::SendFrame(WebpFrame frame)
	{
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_frames.push_back(frame);
	}

	WebpImageComposer::WebpImageComposer()
	{
	}

	WebpImageComposer::~WebpImageComposer()
	{}

	Surface::Ptr WebpImageComposer::ComposeCurrentSurface() {
		return m_frames[0].Surface;
	}

	void WebpImageComposer::OnAdvance() {}

	int WebpImageComposer::OnDelay() {
		return 0;
	}

	void WebpImageComposer::OnRestart() {

	}
}

