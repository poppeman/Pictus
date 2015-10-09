#include "ic_queue.h"

namespace Img {
	void ImageQueue::CacheImage(const ImageList& imagesToProcess) {
		std::lock_guard<std::mutex> l(m_mxPackageList);
		m_workPackages = imagesToProcess;
	}

	void ImageQueue::RemoveImage(Image* image) {
		std::lock_guard<std::mutex> l(m_mxPackageList);
		auto i = std::find(m_workPackages.begin(), m_workPackages.end(), image);
		if(i != m_workPackages.end())
			m_workPackages.erase(i);
	}

	void ImageQueue::Reset() {
		std::lock_guard<std::mutex> l(m_mxPackageList);
		m_workPackages.clear();
	}

	Img::ImageList ImageQueue::GetImageList() {
		std::lock_guard<std::mutex> l(m_mxPackageList);

		return m_workPackages;
	}
}
