#include "webp_imagecomposer.h"

namespace Img {
	void WebpImageComposer::SendFrame(WebpFrame frame)
	{
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_frames.push_back(frame);
	}

	WebpImageComposer::WebpImageComposer():
		m_currFrame(0)
	{
	}

	WebpImageComposer::~WebpImageComposer()
	{}

	Surface::Ptr WebpImageComposer::ComposeCurrentSurface() {
		std::lock_guard<std::mutex> l(m_mutFrames);
		if (m_frames.empty())
		{
			return nullptr;
		}
		return m_frames[m_currFrame].Surface;
	}

	void WebpImageComposer::OnAdvance() {
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_currFrame++;
		if (m_currFrame >= m_frames.size()) {
			m_currFrame = 0;
		}
	}

	int WebpImageComposer::OnDelay() {
		std::lock_guard<std::mutex> l(m_mutFrames);
		if (m_currFrame >= m_frames.size()) {
			return 10;
		}
		return m_frames[m_currFrame].Delay;
	}

	void WebpImageComposer::OnRestart() {
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_currFrame = 0;
	}
}

