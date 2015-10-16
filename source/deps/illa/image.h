#ifndef ILLA_IMAGE_H
#define ILLA_IMAGE_H

#include "types.h"
#include "imagecomposer.h"
#include "../Metadata/Metadata_Document.h"
#include <mutex>
#include <memory>

namespace Img {
	class Image {
	public:
		typedef std::shared_ptr<Image> Ptr;

	public:
		bool IsFinished() const; // Finished = nothing more to do, is true also for corrupt images.
		bool IsHeaderInformationValid() const;

		void SetHeaderData(const Geom::SizeInt& size, ImageComposer::Ptr composer, std::shared_ptr<Metadata::Document> metadata);
		void FinishImage(int loadTime);

		void Deallocate();
		Surface::Ptr CurrentSurface();

		// Valid when header has been successfully loaded at some point
		Geom::SizeInt GetSize() const;
		Geom::SizeInt GetTransformedSize() const;

		std::shared_ptr<Metadata::Document> GetMetadata() const;

		// Valid when State >= StateHeader
		int Delay(); // Will return -1 for static or corrupt images

		void NextFrame();
		void RestartAnimation();

		int LoadTime() const;

		Image();

	private:
		ImageComposer::Ptr m_composer;
		std::shared_ptr<Metadata::Document> m_metadata;

		mutable std::mutex m_mxChangeState;

		int m_loadTime;

		// Cached params
		bool isFinished;
		bool m_isHeaderDataValid;
		Geom::SizeInt m_size;
	};

	Geom::SizeInt CalculateUnzoomedSize(std::shared_ptr<Image> img, Filter::RotationAngle angle);
}

#endif
