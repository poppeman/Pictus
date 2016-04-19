#include "webp_imagecomposer.h"
#include "surfacemgr.h"

namespace Img {
	void WebpImageComposer::SetCanvasSize(Geom::SizeInt newSize)
	{
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_dims = newSize;
	}

	void WebpImageComposer::SetBackgroundColor(Img::Color backgroundColor)
	{
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_backgroundColor = backgroundColor;
	}

	void WebpImageComposer::SetFrameCount(size_t numFrames)
	{
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_numFrames = numFrames;
	}

	void WebpImageComposer::SendFrame(WebpFrame frame)
	{
		std::lock_guard<std::mutex> l(m_mutFrames);
		m_frames.push_back(frame);
	}

	WebpImageComposer::WebpImageComposer():
		m_currFrame(0),
		m_numFrames(0)
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
		if (m_currentSurface == nullptr)
		{
			m_currentSurface = CreateNewSurface(m_dims, Img::Format::ARGB8888);
		}
		if (m_frames[m_currFrame].DisposeMethod == WebpDispose::BackgroundColor)
		{
			m_currentSurface->ClearSurface(m_backgroundColor);
		}
		if (m_frames[m_currFrame].BlendMethod == WebpBlendMethod::Alpha && m_currFrame > 0)
		{
			m_currentSurface->BlitSurfaceAlpha(m_frames[m_currFrame].Surface, m_frames[m_currFrame].Offset);
		}
		else
		{
			m_currentSurface->CopySurface(m_frames[m_currFrame].Surface, m_frames[m_currFrame].Offset);
		}

		return m_currentSurface;
	}

	void WebpImageComposer::OnAdvance() {
		std::lock_guard<std::mutex> l(m_mutFrames);
		if (m_currFrame + 1 < m_frames.size() && m_currentSurface != nullptr) {
			m_currFrame++;
		}
		else if (m_currFrame + 1 == m_numFrames) {
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
