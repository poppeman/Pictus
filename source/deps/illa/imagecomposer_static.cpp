#include "imagecomposer_static.h"

namespace Img {
	Surface::Ptr ImageComposerStatic::ComposeCurrentSurface() {
		if(m_useFastSurface)
			return m_fastSurface;

		m_fastSurface->Deallocate();
		return m_surface;
	}

	ImageComposerStatic::ImageComposerStatic(Surface::Ptr surface, Surface::Ptr fastSurface)
		:m_surface(surface), m_fastSurface(fastSurface), m_useFastSurface(false)
	{}

	void ImageComposerStatic::UseFastSurface( bool doFastSurface ) {
		m_useFastSurface = doFastSurface;
		SurfaceUpdated();
	}

	void ImageComposerStatic::OnRestart() {
		Advance();
	}
}
