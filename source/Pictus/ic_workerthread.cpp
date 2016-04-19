#include "ic_workerthread.h"

namespace Img {
	void DecoderWorkerThread::Queue(ImageQueue::Ptr queue) {
		m_queue = queue;
	}

	void DecoderWorkerThread::Start() {
		if (m_queue == nullptr)
		{
			DO_THROW(Err::InvalidCall, "Queue not set");
		}

		std::lock_guard<std::mutex> l(m_mxWorkList);
		m_thread = std::make_shared<std::thread>(&DecoderWorkerThread::ThreadMain, this);
		m_state = StRunning;
	}

	DecoderWorkerThread::DecoderWorkerThread():m_receiver(0), m_state(StStopped) {}
	DecoderWorkerThread::~DecoderWorkerThread() {
		Terminate();
	}

	void DecoderWorkerThread::ThreadMain() {
		try {
			for (;;) {
				{
					std::unique_lock<std::mutex> l(m_mxWorkList);
wait:
					while(!m_wl.FetchPackage(&m_package) && m_state != StTerminating && m_state != StUpdating)
						m_cdGotWork.wait(l);

					if(m_state == StUpdating) {
						PerformUpdate();
						m_state = StRunning;
						m_cdUpdated.notify_all();
						goto wait;
					}

					if(m_state == StTerminating) {
						m_state = StStopped;
						return;
					}
				}

				if(m_package.Operation() == WOLoadImage)
					processImage(m_package.Loader);

				m_wl.ReturnPackage();
			}
		}
		catch (std::exception& error) {
			SendNotification(Img::MessageReceiver::LoadErrorCritical, 0, error.what());
		}
		catch (...) {
			SendNotification(Img::MessageReceiver::LoadErrorCritical, 0, "Totally, utterly, unknown exception.");
		}
		std::lock_guard<std::mutex> l(m_mxWorkList);
		m_state = StStopped;
	}

	Img::MessageReceiver::LoadMessage DetermineNotification(Img::LoadEvent s) {
		switch(s) {
			case LoadEventHeaderLoaded:
				return MessageReceiver::LoadHeader;
			case LoadEventAllocated:
				return MessageReceiver::LoadAllocated;
			case LoadEventDone:
				return MessageReceiver::LoadDone;
		}
		DO_THROW(Err::InvalidParam, "Unsupported state: " + ToAString(s));
	}

	void DecoderWorkerThread::processImage( ImageLoader::Ptr image ) {
		try {
			LoadEvent e = image->Load();
			if(e != LoadEventNone)
				SendNotification(DetermineNotification(e), image->GetImage());

			if(image->Error())
				SendNotification(Img::MessageReceiver::LoadErrorImage, image->GetImage());
		}
		catch(std::exception& e) {
			SendNotification(Img::MessageReceiver::LoadErrorImage, image->GetImage(), e.what());
		}
	}

	void DecoderWorkerThread::Terminate() {
		std::unique_lock<std::mutex> l(m_mxWorkList);
		if(m_state == StStopped) return;

		if(m_package.Loader)
			m_package.Loader->Abort();

		m_state = StTerminating;
		m_cdGotWork.notify_all();
		l.unlock();
		m_thread->join();
		m_state = StStopped;
	}

	void DecoderWorkerThread::MessageTarget( MessageReceiver* receiver ) {
		m_receiver = receiver;
	}

	void DecoderWorkerThread::Update(bool wait) {
		std::unique_lock<std::mutex> l(m_mxWorkList);
		if(m_state == StStopped)
			return;

		m_state = StUpdating;
		if(m_package.Loader)
			m_package.Loader->Abort();
		m_cdGotWork.notify_all();
		if(wait && m_state != StStopped)
			m_cdUpdated.wait(l);
	}


	IO::FileReader::Ptr DecoderWorkerThread::GetFileReader(const std::string& filename) {
		std::lock_guard<std::mutex> l(m_mxWorkList);
		return m_wl.GetFileReader(filename);
	}

	void DecoderWorkerThread::SendNotification(Img::MessageReceiver::LoadMessage msg, Img::Image* image, const std::string& desc) {
		if (m_receiver) {
			m_receiver->OnLoadMessage(msg, image, desc);
		}
	}

	void DecoderWorkerThread::MemoryLimit( size_t newLimit ) {
		m_wl.MemoryLimit(newLimit);
	}

	void DecoderWorkerThread::PerformUpdate() {
		m_wl.Rebuild(m_queue->GetImageList());
		m_package.Loader.reset();
	}

	void DecoderWorkerThread::SetCodecFactoryStore(Img::CodecFactoryStore* cfs) {
		m_wl.SetCodecFactoryStore(cfs);
	}
}

