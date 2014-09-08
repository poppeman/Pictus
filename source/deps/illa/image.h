#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include "imagecomposer.h"

namespace Img {
	class Image {
	public:
		typedef std::shared_ptr<Image> Ptr;

	public:
		_Check_return_ bool IsFinished() const; // Finished = nothing more to do, is true also for corrupt images.
		_Check_return_ bool IsHeaderInformationValid() const;

		void SetHeaderData(_In_ const Geom::SizeInt& size, _In_ ImageComposer::Ptr composer);
		void FinishImage(_In_ int loadTime);

		void Deallocate();
		_Check_return_ Surface::Ptr CurrentSurface();

		// Valid when header has been successfully loaded at some point.
		_Check_return_ Geom::SizeInt GetSize() const;

		// Valid when State >= StateHeader
		_Check_return_ int Delay(); // Will return -1 for static or corrupt images

		void NextFrame();
		void RestartAnimation();

		_Check_return_ int LoadTime() const;

		Image(std::wstring filename);

	private:
		ImageComposer::Ptr m_composer;

		mutable std::mutex m_mxChangeState;

		int m_loadTime;

		// Cached params
		bool isFinished;
		bool m_isHeaderDataValid;
		Geom::SizeInt m_size;
	};
}

#endif
