#include "imagecomposer_gif.h"
#include "surfacemgr.h"

namespace Img {
	Palette DeterminePalette(ImageDescriptor::Ptr currId, Palette globalPalette) {
		auto currPal = (currId->LCTExist ? currId->LCTEntries : globalPalette);
		int cKey = currId->ColorKey;
		if (currId->IsTransparent) {
			Color c = currPal.Color(cKey);
			c.A = 0;
			currPal.Color(cKey, c);
		}
		return currPal;
	}

	Surface::Ptr ImageComposerGif::ComposeCurrentSurface() {
		if (m_ids == nullptr) {
			return Surface::Ptr();
		}

		auto currId = (*m_ids)[m_currentFrame];
		auto currPal = DeterminePalette(currId, m_globalPalette);
		currId->Surface->SetPalette(currPal);

		if (m_ids->size() == 1) {
			return currId->Surface;
		}

		if (m_currentSurface == 0) {
			m_currentSurface = CreateNewSurface(m_dims, Img::Format::ARGB8888);
			m_lastUndisposed = CreateNewSurface(m_dims, Img::Format::ARGB8888);
		}

		if (m_currentFrame == 0) {
			m_currentSurface->ClearSurface({ 0, 0, 0, 0 });
			m_lastUndisposed->ClearSurface({ 0, 0, 0, 0 });
		}

		if (m_frameLoading <= m_currentFrame) {
			return m_currentSurface;
		}

		// GIF disposal method indicates how the CURRENT frame should be cleaned up after rendering but this is
		// slightly impractical so instead we handle the PREVIOUS frame's disposal here. End result should be the same.
		auto disposeMethod = ImageDescriptor::DisposeUnknown;
		if (m_currentFrame > 0) {
			disposeMethod = (*m_ids)[m_currentFrame - 1]->Dispose;
		}

		if (disposeMethod == ImageDescriptor::DisposeNoDispose || disposeMethod == ImageDescriptor::DisposeUnknown) {
			m_lastUndisposed->CopySurface(m_currentSurface);
		}
		if (disposeMethod == ImageDescriptor::DisposeToPrevious) {
			m_currentSurface->CopySurface(m_lastUndisposed);
		}
		if (disposeMethod == ImageDescriptor::DisposeBackground) {
			m_currentSurface->FillRect(Geom::RectInt((*m_ids)[m_currentFrame - 1]->TopLeft, (*m_ids)[m_currentFrame - 1]->ImageSize), currPal.Color(currId->ColorKey));
		}

		if (currId->IsTransparent) {
			m_currentSurface->BlitSurfaceColorKey(currId->Surface, currId->TopLeft, currId->ColorKey);
		}
		else {
			m_currentSurface->CopySurface(currId->Surface, currId->TopLeft);
		}

		m_isRendered = true;
		return m_currentSurface;
	}

	void ImageComposerGif::OnAdvance() {
		if (m_isRendered == false) {
			return;
		}

		if ((m_currentFrame + 1) >= m_ids->size()) {
			m_isRendered = false;
			m_currentFrame = 0;
		}
		else if (m_frameLoading > (m_currentFrame + 1)) {
			m_isRendered = false;
			m_currentFrame++;
		}
	}

	int ImageComposerGif::OnDelay() {
		if (m_ids == nullptr) {
			return 10;
		}
		if (m_ids->size() == 1) {
			return -1;
		}
		if (m_frameLoading <= m_currentFrame || !m_isRendered) {
			return 10;
		}
		if (m_currentFrame < m_ids->size()) {
			return (*m_ids)[m_currentFrame]->Delay;
		}
		return -1;
	}

	void ImageComposerGif::OnRestart() {
		m_currentFrame = 0;
		m_isRendered = false;
		ComposeCurrentSurface();
	}

	ImageComposerGif::ImageComposerGif():
		m_frameLoading{ 0 },
		m_currentFrame{ 0 },
		m_isRendered{ false }
	{}

	void ImageComposerGif::SendImageData(DescriptorVectorPtr descriptors, const Geom::SizeInt& imageDims, const Palette& globalPalette) {
		m_ids = descriptors;
		m_dims = imageDims;
		m_globalPalette = globalPalette;
	}

	void ImageComposerGif::FrameFinished( int frameIndex ) {
		m_frameLoading = frameIndex;
	}
}
