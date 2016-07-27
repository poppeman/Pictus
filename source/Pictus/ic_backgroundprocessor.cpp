#include "ic_backgroundprocessor.h"

namespace Img {
	using Img::Image;

	void BackgroundProcessor::MessageTarget(Img::MessageReceiver* receiver) {
		std::lock_guard<std::mutex> l(m_mutexReportWindow);
		m_workerThread->MessageTarget(receiver);
	}

	void BackgroundProcessor::CacheImage(const ImageList& newImageList) {
		m_queue->CacheImage(newImageList);
		m_workerThread->Update();
	}

	void BackgroundProcessor::RemoveImage(Img::Image* image) {
		m_queue->RemoveImage(image);
		m_workerThread->Update(true);
	}

	void BackgroundProcessor::Reset() {
		m_queue->Reset();
		m_workerThread->Update(true);
	}

	BackgroundProcessor::BackgroundProcessor():m_queue(new ImageQueue), m_workerThread(new DecoderWorkerThread) {
		m_workerThread->Queue(m_queue);
		m_workerThread->Start();
	}

	BackgroundProcessor::~BackgroundProcessor() {
		m_workerThread->Terminate();
	}

	size_t BackgroundProcessor::AllocatedMemory() const {
		return 0;
	}

	void BackgroundProcessor::MemoryLimit(size_t limit) {
		m_workerThread->MemoryLimit(limit);
		m_workerThread->Update();
	}

	void BackgroundProcessor::SetMaximumResolutionHint( const Geom::SizeInt& maxRes ) {
		ImageLoader::SetMaximumResolutionHint(maxRes);
	}

	IO::FileReader::Ptr BackgroundProcessor::GetFileReader(const std::string& filename) {
		return m_workerThread->GetFileReader(filename);
	}

	void BackgroundProcessor::SetCodecFactoryStore(Img::CodecFactoryStore* cfs) {
		m_workerThread->SetCodecFactoryStore(cfs);
	}
}
