#ifndef PICTUS_IMAGECACHE_WORKERTHREAD_H
#define PICTUS_IMAGECACHE_WORKERTHREAD_H

#include "ic_queue.h"
#include "ic_worklist.h"
#include "imagecache_event.h"
#include <thread>
#include <condition_variable>

namespace Img {
	class DecoderWorkerThread {
	public:
		void Queue(ImageQueue::Ptr queue);
		void Start();
		void Update(bool wait = false);

		void SetCodecFactoryStore(Img::CodecFactoryStore* cfs);

		IO::FileReader::Ptr GetFileReader(const std::string& filename);

		void Terminate();

		void MemoryLimit(size_t newLimit);

		void MessageTarget(MessageReceiver* receiver);

		DecoderWorkerThread();
		~DecoderWorkerThread();

		typedef std::shared_ptr<DecoderWorkerThread> Ptr;

	private:
		void SendNotification(Img::MessageReceiver::LoadMessage msg, Img::Image* image, const std::string& desc = "");

		void ThreadMain();

		void PerformUpdate();
		void processImage( ImageLoader::Ptr image );
		ImageQueue::Ptr m_queue;
		std::shared_ptr<std::thread> m_thread;

		enum State {
			StStopped,
			StRunning,
			StUpdating,
			StResuming,
			StTerminating,
		};

		MessageReceiver* m_receiver = nullptr;
		State m_state;

		std::mutex m_mxWorkList;
		std::condition_variable m_cdGotWork;
		std::condition_variable m_cdUpdated;
		std::condition_variable m_cdRunning;

		bool m_isAlive = false;

		WorkList m_wl;
		WorkPackage m_package;
	};
}

#endif
