#ifndef IMAGECOMPOSER_H
#define IMAGECOMPOSER_H

#include "surface.h"
#include <mutex>

namespace Img {
	class ImageComposer {
	public:
		Surface::Ptr RequestCurrentSurface();
		void Advance();
		int Delay();
		void Restart();

		ImageComposer();
		virtual ~ImageComposer()=0;

		typedef std::shared_ptr<ImageComposer> Ptr;

		ImageComposer(const ImageComposer&) = delete;
		ImageComposer& operator=(const ImageComposer&) = delete;

	protected:
		void SurfaceUpdated();

	private:
		void Compose();

		virtual Surface::Ptr ComposeCurrentSurface()=0;
		virtual void OnAdvance();
		virtual int OnDelay();
		virtual void OnRestart();

		std::mutex m_mxRecompose;
		bool m_recompose;
		Surface::Ptr m_currentSurface;
	};
}

#endif
