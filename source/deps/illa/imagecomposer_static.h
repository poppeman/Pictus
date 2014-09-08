#ifndef IMAGECOMPOSER_STATIC_H
#define IMAGECOMPOSER_STATIC_H

#include "imagecomposer.h"

namespace Img {
	class StaticCodec;

	class ImageComposerStatic:public ImageComposer {
	public:
		ImageComposerStatic(Surface::Ptr surface, Surface::Ptr fastSurface);

		void UseFastSurface(bool doFastSurface);

		typedef std::shared_ptr<ImageComposerStatic> Ptr;

	private:
		Surface::Ptr ComposeCurrentSurface() override;
		void OnRestart() override;

		Surface::Ptr m_surface;
		Surface::Ptr m_fastSurface;

		bool m_useFastSurface;
	};
}


#endif
