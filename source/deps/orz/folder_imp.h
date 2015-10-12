#ifndef ORZ_FOLDER_IMP_H
#define ORZ_FOLDER_IMP_H

#include "folder_iterator.h"

namespace IO {
	class FolderImplementation:boost::noncopyable {
	public:
		const std::string& Path() const;
		virtual FolderFileIterator::Ptr CreateIterator()=0;

		FolderImplementation(const std::string& path);

	private:
		std::string m_path;
	};
}

#endif
