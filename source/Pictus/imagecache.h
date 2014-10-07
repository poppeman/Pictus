#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "orz/folder.h"
#include "imagecache_event.h"
#include "ic_backgroundprocessor.h"
#include "imagecache_fileentry.h"

namespace Img {
	class Image;
	class CodecFactoryStore;

	class Cacher {
	public:
		enum SortMethod {
			SortNothing,
			SortFilename,
			SortDateModified,
			SortDateAccessed,
			SortDateCreated
		};

		void Clear();

		void SetMaximumResolutionHint(const Geom::SizeInt& hint);

		Img::Image::Ptr Sort(SortMethod method);
		void MessageTarget(MessageReceiver* pWatcher);
		Img::Image::Ptr AddImageLast(const std::wstring& filename);
		Img::Image::Ptr RemoveCurrentImage();
		Img::Image::Ptr RemoveImageIndex(size_t index);
		Img::Image::Ptr RemoveImageFilename(const std::wstring& filename);

		bool RenamedImage(const std::wstring& oldFilename, const std::wstring& newFilename);

		IO::FileReader::Ptr CurrentImageFileReader();

		size_t CurrentImageIndex();
		size_t ImageCount();

		Img::Image::Ptr FirstImage();

		Img::Image::Ptr PreviousImage();
		Img::Image::Ptr NextImage();

		Img::Image::Ptr LastImage();

		Img::Image::Ptr CurrentImage();
		FileInt CurrentImageLastModifiedDate();
		FileInt CurrentImageFileSize();
		std::wstring CurrentImageFilename() const;

		Img::Image::Ptr PeekRelative(int delta);
		Img::Image::Ptr PeekAt(size_t position);

		Img::Image::Ptr GotoImage(size_t index);
		bool FindImage(const std::wstring& name, size_t* index);

		void MemoryLimit(size_t limit);

		size_t AllocatedMemory();

		void Construct();
		void Stop();

		void WrapAround(bool doWrap);

		void SetCodecFactoryStore(Img::CodecFactoryStore* cfs);

		Cacher();

	private:
		Img::Image::Ptr apply_priorities(bool flip=false);

		WorkRequest PeekRelativeRequest(int delta);
		bool PeekRelativeIndex(int delta, size_t* index);

	private:
		typedef std::vector<Internal::FileEntry> FileList;

		FileList m_files;
		size_t m_index;
		bool m_wentForward;
		bool m_wrapAround;

		std::auto_ptr<BackgroundProcessor> m_decThread;
		MessageReceiver* m_pReceiver;

		Img::CodecFactoryStore* m_cfs;
	};

	bool FillCacher(Cacher& cache, IO::Folder& folder);
}

#endif
