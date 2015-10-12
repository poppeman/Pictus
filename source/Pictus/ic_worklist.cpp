#include "ic_worklist.h"
#include "ic_imageloader.h"

namespace Img {
	void WorkList::Rebuild(const ImageList& imagesToProcess) {
		m_memoryLimit = m_newMemoryLimit;

		PackageList newPackages;

		for (ImageList::const_iterator newBatchEntry = imagesToProcess.begin(); newBatchEntry != imagesToProcess.end(); ++newBatchEntry) {
			if (*newBatchEntry == 0)
				continue;

			auto oldQueueEntry = std::find(m_workPackages.begin(), m_workPackages.end(), (*newBatchEntry).image);

			if (oldQueueEntry == m_workPackages.end()) {
				WorkPackage newEntry(std::make_shared<ImageLoader>(m_cfs, (*newBatchEntry).image, (*newBatchEntry).filename));
				newPackages.push_back(newEntry);
			}
			else {
				newPackages.push_back(*oldQueueEntry);
				m_workPackages.erase(oldQueueEntry);
			}
		}

		for(auto& p: m_workPackages)
			p.Loader->GetImage()->Deallocate();

		m_workPackages = newPackages;

		size_t currUsed = 0;
		bool first = true;
		for(auto& p: m_workPackages) {
			p.Loader->AllowFastSkip(first);

			if(p.Operation() == WODone || (p.Operation() == WOLoadImage && p.Loader->GetState() >= LoadStateLoading)) {
				if(!first && (currUsed + p.MemoryUsage() > m_memoryLimit))
					p.Loader->Deallocate();
				else
					currUsed += p.MemoryUsage();
			}
			first = false;
		}
	}

	bool WorkList::ScanForPackage(WorkPackage* package, bool fastPass) {
		size_t currUsed = 0;

		for(auto& p: m_workPackages) {
			if(SuitableWorkPackage(p, currUsed, fastPass)) {
				*package = p;
				return true;
			}
			else currUsed +=  p.MemoryUsage();
		}
		return false;
	}

	bool WorkList::FetchPackage( WorkPackage* package ) {
		bool ret = LocatePackage(package);
		m_currPackage = package;
		return ret;
	}

	void WorkList::ReturnPackage() {
		if(m_workPackages.empty()) return;
		if(!m_currPackage) return;

		m_currPackage->Loader->ResetAbort();
	}

	WorkList::~WorkList() {
		for(auto& p: m_workPackages)
			p.Loader->GetImage()->Deallocate();
	}

	WorkList::WorkList():m_currPackage(0), m_memoryLimit(0), m_cfs(0)
	{
	}

	void WorkList::MemoryLimit( size_t newLimit ) {
		m_newMemoryLimit = newLimit;
	}

	bool WorkList::SuitableWorkPackage(const WorkPackage& p, size_t currUsed, bool fastPass) {
		if(p.Operation() != WOLoadImage) return false;
		if(p.Loader->GetState() == LoadStateUnprocessed)
			return true;
		if(currUsed == 0) return true; // Always attempt to load at least ONE image.
		if(fastPass && !p.FastPass()) return false;
		if(currUsed + p.MemoryUsage() < m_memoryLimit) return true;
		return false;
	}

	IO::FileReader::Ptr WorkList::GetFileReader(const std::string& filename) {
		for(auto& p: m_workPackages) {
			if(!p.Loader) continue;
			if(p.Loader->Filename() == filename)
				return p.Loader->GetFileReader();
		}
		return IO::FileReader::Ptr();
	}

	void WorkList::SetCodecFactoryStore(Img::CodecFactoryStore* cfs) {
		m_cfs = cfs;
	}

	bool WorkList::LocatePackage(WorkPackage* package) {
		if(m_workPackages.empty()) return false;

		if(ScanForPackage(package, true))
			return true;

		return (ScanForPackage(package, false));
	}
}
