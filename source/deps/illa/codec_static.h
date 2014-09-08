#ifndef STATICCODEC_H
#define STATICCODEC_H

#include "codec.h"
#include "imagecomposer_static.h"

namespace Img {
	class StaticCodec:public AbstractCodec {
	public:
		ImageComposer::Ptr RequestImageComposer();

		StaticCodec();
		virtual ~StaticCodec();

	protected:
		void UseFastSurface(bool doFastSurface);
		void FinalizeSurface();

		Surface::Ptr GetSurface();

	private:
		ImageComposerStatic::Ptr m_composer;
		Surface::Ptr m_surface;
		Surface::Ptr m_fastSurface;
		bool m_doFast;
	};
}

#endif
