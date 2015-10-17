#include "imagecache.h"
#include "ic_backgroundprocessor.h"
#include "illa/codecmgr.h"
#include "orz/fileops.h"
#include "orz/stream_file.h"
#include <Shlwapi.h>

namespace Img {
	using namespace Geom;

	struct fnSortWinapiFile {
		bool operator () (Internal::FileEntry& a, Internal::FileEntry& b) {
			return (StrCmpLogicalW(UTF8ToWString(IO::GetFile(a.Name())).c_str(), UTF8ToWString(IO::GetFile(b.Name())).c_str())==-1);
		}
	};

	struct fnSortDateModified {
		bool operator () (Internal::FileEntry& a, Internal::FileEntry& b) {	return (a.DateModified() < b.DateModified()); }
	};

	struct fnSortDateAccessed {
		bool operator () (Internal::FileEntry& a, Internal::FileEntry& b) {	return (a.DateAccessed() < b.DateAccessed()); }
	};

	struct fnSortDateCreated {
		bool operator () (Internal::FileEntry& a, Internal::FileEntry& b) {	return (a.DateCreated() < b.DateCreated()); }
	};

	void attemptPush(ImageList& queue, WorkRequest newRequest) {
		if (std::find(queue.begin(), queue.end(), newRequest) == queue.end()) {
			queue.push_back(newRequest);
		}
	}

	Img::Image::Ptr Cacher::apply_priorities(bool flip) {
		Img::Image::Ptr image = CurrentImage();

		// Highest priority on current, higher on next, high on prev, medium on next-next, minimum on prev-prev.
		ImageList newQueue;
		newQueue.push_back(WorkRequest(image.get(), CurrentImageFilename()));

		if (flip == false) {
			attemptPush(newQueue, PeekRelativeRequest(1));
			attemptPush(newQueue, PeekRelativeRequest(-1));
			attemptPush(newQueue, PeekRelativeRequest(2));
			attemptPush(newQueue, PeekRelativeRequest(-2));
		}
		else {
			attemptPush(newQueue, PeekRelativeRequest(-1));
			attemptPush(newQueue, PeekRelativeRequest(1));
			attemptPush(newQueue, PeekRelativeRequest(-2));
			attemptPush(newQueue, PeekRelativeRequest(2));
		}

		m_decThread->CacheImage(newQueue);

		return image;
	}

	void Cacher::Clear() {
		m_decThread->Reset();
		m_files.clear();

		m_index = 0;
		m_wentForward = true;
	}

	Img::Image::Ptr Cacher::Sort(SortMethod method) {
		std::string currentImageFile;
		if (m_index > 0 && m_index < m_files.size()) {
			currentImageFile = m_files[m_index].Name();
		}

		//Image::Ptr currentImage = CurrentImage();

		switch(method) {
		case SortFilename:
			std::sort(m_files.begin(), m_files.end(), fnSortWinapiFile());
			break;
		case SortDateModified:
			std::sort(m_files.begin(), m_files.end(), fnSortDateModified());
			break;
		case SortDateAccessed:
			std::sort(m_files.begin(), m_files.end(), fnSortDateAccessed());
			break;
		case SortDateCreated:
			std::sort(m_files.begin(), m_files.end(), fnSortDateCreated());
			break;
		}

		if (currentImageFile.empty() == false) {
			size_t index = 0;
			if (FindImage(currentImageFile, &index)) {
				GotoImage(index);
			}
		}

		return apply_priorities();
	}


	void Cacher::MessageTarget(MessageReceiver* pWatcher) {
		m_pReceiver = pWatcher;
		m_decThread->MessageTarget(pWatcher);
	}

	Img::Image::Ptr Cacher::AddImageLast(const std::string& filename) {
		if (m_cfs == nullptr)
		{
			DO_THROW(Err::InvalidCall, "SetCodecFactoryStore not yet called.");
		}

		if (m_cfs->DoCodecExist(IO::GetExtension(filename).c_str())) {
			m_files.push_back(Internal::FileEntry(filename));

			if (ImageCount() == 1) {
				m_index = 0;
				m_wentForward = true;
			}

			return apply_priorities();
		}
		else
			return CurrentImage();
	}

	Img::Image::Ptr Cacher::RemoveCurrentImage() {
		if (ImageCount() == 0)
		{
			DO_THROW(Err::InvalidCall, "Cacher is already empty.");
		}

		m_decThread->RemoveImage(CurrentImage().get());
		m_files.erase(m_files.begin() + m_index);
		if (m_index >= ImageCount() || ((m_wentForward == false) && (m_index > 0)))
			--m_index;

		return apply_priorities();
	}

	Img::Image::Ptr Cacher::RemoveImageIndex(size_t index) {
		if (index >= ImageCount())
		{
			DO_THROW(Err::InvalidParam, "index out of bounds");
		}

		if (index == m_index) return RemoveCurrentImage();

		m_decThread->RemoveImage(PeekAt(index).get());
		m_files.erase(m_files.begin() + index);

		if (m_index >= ImageCount() || (m_index > index))
			--m_index;

		// Start working at the new position
		return apply_priorities();
	}

	Img::Image::Ptr Cacher::RemoveImageFilename(const std::string& filename) {
		size_t index;
		if (FindImage(filename, &index))
			return RemoveImageIndex(index);
		else
			return CurrentImage();
	}

	bool Cacher::RenamedImage(const std::string& oldFilename, const std::string& newFilename) {
		size_t index;
		if(!FindImage(oldFilename, &index))
			return false;

		IO::FileReader::Ptr r = m_decThread->GetFileReader(oldFilename);
		if(r) {
			IO::StreamFile* f = dynamic_cast<IO::StreamFile*>(r->GetStream());
			if(f)
				f->Renamed(newFilename);
		}

		m_files[index].Rename(newFilename);
		return true;
	}

	IO::FileReader::Ptr Cacher::CurrentImageFileReader() {
		IO::FileReader::Ptr f = m_decThread->GetFileReader(CurrentImageFilename());
		if(f) return f;
		return IO::FileReader::Ptr(new IO::FileReader(CurrentImageFilename()));
	}

	size_t Cacher::CurrentImageIndex() {
		return m_index;
	}

	size_t Cacher::ImageCount() {
		return m_files.size();
	}

	Img::Image::Ptr Cacher::FirstImage() {
		// Update database position
		m_index = 0;
		m_wentForward = true;

		apply_priorities(false);

		return CurrentImage();
	}

	Img::Image::Ptr Cacher::PreviousImage() {
		if (ImageCount() == 0) return Img::Image::Ptr();

		// Step back
		m_wentForward = false;
		if (m_wrapAround == false) {
			if (m_index > 0) --m_index;
		}
		else {
			if (m_index == 0)
				m_index = ImageCount();

			--m_index;
		}

		// Update cacher
		return apply_priorities(true);
	}

	Img::Image::Ptr Cacher::NextImage() {
		if (ImageCount() == 0) return Img::Image::Ptr();

		m_wentForward = true;

		if (m_wrapAround == false) {
			if ((m_index + 1) < ImageCount())
				++m_index;
		}
		else {
			++m_index;
			if (m_index >= ImageCount())
				m_index = 0;
		}

		// Update cacher
		return apply_priorities();
	}

	Img::Image::Ptr Cacher::LastImage() {
		// Update database position
		m_index = ImageCount() - 1;
		m_wentForward = false;

		apply_priorities(true);

		return CurrentImage();
	}

	Img::Image::Ptr Cacher::GotoImage(size_t index) {
		size_t numImages = ImageCount();

		if (numImages <= 0)
		{
			DO_THROW(Err::InvalidCall, "index was out of bounds.");
		}

		if (index >= numImages)
		{
			DO_THROW(Err::InvalidParam, "index was out of bounds.");
		}

		m_wentForward = true;
		m_index = index;

		apply_priorities();

		return CurrentImage();
	}

	bool Cacher::FindImage(const std::string& name, size_t *index) {
		if (name.empty())
		{
			DO_THROW(Err::InvalidParam, "name was empty");
		}

		for(size_t i = 0; i < ImageCount(); ++i) {
			if (m_files.at(i).Name() == name) {
				if (index != 0)
					*index = i;
				return true;
			}
		}
		return false;
	}

	Img::Image::Ptr Cacher::CurrentImage() {
		if (ImageCount() == 0) return Img::Image::Ptr();
		return m_files[m_index].Image();
	}

	Img::Image::Ptr Cacher::PeekRelative(int delta) {
		size_t index;
		if(!PeekRelativeIndex(delta, &index))
			return Img::Image::Ptr();

		return m_files[index].Image();
	}

	Img::Image::Ptr Cacher::PeekAt(size_t position) {
		if (position >= ImageCount())
		{
			DO_THROW(Err::InvalidParam, "position out of bounds.");
		}

		return m_files[position].Image();
	}

	void Cacher::MemoryLimit(size_t limit) {
		m_decThread->MemoryLimit(limit);
		apply_priorities();
	}

	size_t Cacher::AllocatedMemory() {
		return m_decThread->AllocatedMemory();
	}

	void Cacher::Stop() {
		m_decThread.reset(0);
	}

	Cacher::Cacher()
		:m_pReceiver(0),
		 m_decThread(new BackgroundProcessor()),
		 m_wentForward(true),
		 m_wrapAround(false),
		 m_cfs(0)
	{}

	void Cacher::Construct() {
		m_decThread.reset(new BackgroundProcessor());
		m_files.clear();
		m_index = 0;
		m_wentForward = true;
	}

	void Cacher::WrapAround( bool doWrap ) {
		m_wrapAround = doWrap;
	}

	void Cacher::SetMaximumResolutionHint( const Geom::SizeInt& hint ) {
		m_decThread->SetMaximumResolutionHint(hint);
	}


	FileInt Cacher::CurrentImageLastModifiedDate() {
		if (m_index >= m_files.size()) return 0;
		return m_files[m_index].DateModified();
	}

	FileInt Cacher::CurrentImageFileSize() {
		if (m_index >= m_files.size()) return 0;
		return m_files[m_index].FileSize();
	}

	std::string Cacher::CurrentImageFilename() const
	{
		if (m_index >= m_files.size()) return "";
		return m_files[m_index].Name();
	}

	Img::WorkRequest Cacher::PeekRelativeRequest(int delta) {
		size_t index;
		if(!PeekRelativeIndex(delta, &index)) return Img::WorkRequest(0, "");
		return WorkRequest(m_files[index].Image().get(), m_files[index].Name());
	}

	bool Cacher::PeekRelativeIndex(int delta, size_t* index) {
		if (delta == 0) {
			*index = CurrentImageIndex();
			return true;
		}

		int currentImagePosition = (static_cast<int>(m_index) + delta);
		if (currentImagePosition < 0 && m_wrapAround) currentImagePosition = ImageCount() + currentImagePosition;
		if(static_cast<size_t>(currentImagePosition) >= ImageCount() && m_wrapAround) currentImagePosition = currentImagePosition - ImageCount();
		if (currentImagePosition < 0) return false;
		if(static_cast<size_t>(currentImagePosition) >= ImageCount()) return false;

		*index = currentImagePosition;
		return true;
	}

	void Cacher::SetCodecFactoryStore(Img::CodecFactoryStore* cfs) {
		m_cfs = cfs;
		m_decThread->SetCodecFactoryStore(cfs);
	}

	bool FillCacher(Cacher& cache, IO::Folder& folder) {
		cache.Clear();

		IO::FileList contents = folder.CurrentContents();
		for (const auto& i : contents)
			cache.AddImageLast(folder.Path() + i.Name);

		cache.Sort(Cacher::SortFilename);

		return true;
	}
}
