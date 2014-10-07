#ifndef IMAGECOMPOSER_H
#define IMAGECOMPOSER_H

#include "surface.h"

namespace Img {
	class ImageComposer:boost::noncopyable {
	public:
		Surface::Ptr RequestCurrentSurface();
		void Advance();
		int Delay();
		void Restart();

		ImageComposer();
		virtual ~ImageComposer()=0;

		typedef std::shared_ptr<ImageComposer> Ptr;

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
