
#ifndef IMAGECACHE_THREAD_H
#define IMAGECACHE_THREAD_H

#include "imagecache_event.h"
#include "ic_queue.h"
#include "imagecache_shared.h"
#include "ic_workerthread.h"

namespace Img {
	class Image;
};

namespace Img { 
	class BackgroundProcessor {
	public:
		void MessageTarget(MessageReceiver* pWatcher);

		void CacheImage(const ImageList& newImageList);
		void RemoveImage(Img::Image* image);

		IO::FileReader::Ptr GetFileReader(const std::wstring& filename);

		const size_t AllocatedMemory() const;
		void MemoryLimit(size_t limit);

		void SetMaximumResolutionHint(const Geom::SizeInt& maxRes);
		void SetCodecFactoryStore(Img::CodecFactoryStore* cfs);

		void Reset();

		BackgroundProcessor();
		~BackgroundProcessor();

	private:
		std::mutex m_mutexReportWindow;

		ImageQueue::Ptr m_queue;

		DecoderWorkerThread::Ptr m_workerThread;
	};
}

#endif
