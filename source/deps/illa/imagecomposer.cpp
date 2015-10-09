#include "imagecomposer.h"

namespace Img {
	Surface::Ptr ImageComposer::RequestCurrentSurface() {
		if(m_currentSurface == 0)
			Compose();
		else {
			std::lock_guard<std::mutex> l(m_mxRecompose);

			if (m_recompose) {
				Compose();
			}
			m_recompose = false;
		}

		return m_currentSurface;
	}

	void ImageComposer::Advance() {
		OnAdvance();
		Compose();
	}

	int ImageComposer::Delay() {
		return OnDelay();
	}

	void ImageComposer::Restart() {
		return OnRestart();
	}

	void ImageComposer::OnAdvance() {}

	int ImageComposer::OnDelay() {
		return -1;
	}

	void ImageComposer::OnRestart() {}

	void ImageComposer::Compose() {
		m_currentSurface = ComposeCurrentSurface();
	}

	void ImageComposer::SurfaceUpdated() {
		std::lock_guard<std::mutex> l(m_mxRecompose);
		m_recompose = true;
	}

	ImageComposer::ImageComposer():m_recompose(false) {}
    ImageComposer::~ImageComposer() {}
}
