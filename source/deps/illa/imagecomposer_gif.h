#ifndef IMAGECOMPOSER_GIF_H
#define IMAGECOMPOSER_GIF_H

#include "imagecomposer.h"
#include "gif_imagedescriptor.h"

namespace Img {
	class ImageComposerGif:public ImageComposer {
	public:
		ImageComposerGif();

		// TODO: Use some kind of AddImageDescriptor instead of just tossing a pointer to the entire list.
		void SendImageData(DescriptorVectorPtr descriptors, const Geom::SizeInt& imageDims, const Palette& globalPalette);
		void FrameFinished(int frameIndex);

		typedef std::shared_ptr<ImageComposerGif> Ptr;

	private:
		Surface::Ptr ComposeCurrentSurface() override;
		void OnAdvance() override;
		int OnDelay() override;
		void OnRestart() override;

		size_t m_currentFrame;
		size_t m_frameLoading;
		bool m_isRendered;
		DescriptorVectorPtr m_ids;
		Geom::SizeInt m_dims;
		Palette m_globalPalette;
		Surface::Ptr m_currentSurface;
		Surface::Ptr m_lastUndisposed;
	};
}

#endif
