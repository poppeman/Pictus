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

		WorkList();
		~WorkList();

	private:
		bool LocatePackage(WorkPackage* package);
		bool SuitableWorkPackage(const WorkPackage& p, size_t currUsed, bool fastPass);
		bool ScanForPackage(WorkPackage* package, bool fastPass);

		typedef std::list<WorkPackage> PackageList;

		size_t m_memoryLimit;
		CodecFactoryStore* m_cfs;
		PackageList m_workPackages;
		WorkPackage* m_currPackage;
		size_t m_newMemoryLimit;
	};
}

#endif
