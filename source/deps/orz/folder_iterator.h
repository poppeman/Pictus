#ifndef ORZ_FOLDER_ITERATOR_H
#define ORZ_FOLDER_ITERATOR_H

#include "folder_types.h"

namespace IO {
	class FolderFileIterator {
	public:
		bool Step();
		FolderEntry CurrentEntry();

		typedef std::shared_ptr<FolderFileIterator> Ptr;

		virtual ~FolderFileIterator();
	
	private:
		virtual bool OnStep()=0;
		virtual FolderEntry OnCurrentEntry()=0;
	};
}

#endif
