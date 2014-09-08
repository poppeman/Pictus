#include "StdAfx.h"
#include "codec_static.h"
#include "imagecomposer_static.h"
#include "surfacemgr.h"

namespace Img {
	ImageComposer::Ptr StaticCodec::RequestImageComposer() {
		return m_composer;
	}

	StaticCodec::StaticCodec():m_surface(CreateNewSurface()), m_fastSurface(CreateNewSurface()), m_doFast(false) {
		m_composer = std::make_shared<ImageComposerStatic>(m_surface, m_fastSurface);
	}

	StaticCodec::~StaticCodec() {}

	void StaticCodec::UseFastSurface( bool doFastSurface ) {
		m_doFast = doFastSurface;
		if (doFastSurface) {
			m_composer->UseFastSurface(true);
		}
	}

	void StaticCodec::FinalizeSurface() {
		if (!m_doFast) {
			m_composer->UseFastSurface(false);
		}
	}

	Surface::Ptr StaticCodec::GetSurface() {
		return m_doFast ? m_fastSurface : m_surface;
	}
}
