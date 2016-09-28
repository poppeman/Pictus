#ifndef PICTUS_IC_WORKLIST_H
#define PICTUS_IC_WORKLIST_H

#include "ic_workpackage.h"
#include "imagecache_workrequest.h"
#include <list>

namespace Img {
	class WorkList {
	public:
		void Rebuild(const ImageList& newList);
		bool FetchPackage(WorkPackage* package);

		void ReturnPackage();
		void MemoryLimit(size_t newLimit);

		IO::FileReader::Ptr GetFileReader(const std::string& filename);

		void SetCodecFactoryStore(Img::CodecFactoryStore* cfs);

		~WorkList();

	private:
		bool LocatePackage(WorkPackage* package);
		bool SuitableWorkPackage(const WorkPackage& p, size_t currUsed, bool fastPass);
		bool ScanForPackage(WorkPackage* package, bool fastPass);

		typedef std::list<WorkPackage> PackageList;

		WorkPackage* m_currPackage = nullptr;
		size_t m_memoryLimit = 0;
		size_t m_newMemoryLimit = 0;
		CodecFactoryStore* m_cfs = nullptr;
		PackageList m_workPackages;
	};
}

#endif
