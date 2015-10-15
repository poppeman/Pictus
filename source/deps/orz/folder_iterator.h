#ifndef ORZ_FOLDER_ITERATOR_H
#define ORZ_FOLDER_ITERATOR_H

#include "folder_types.h"
#include <boost/filesystem.hpp>

namespace IO {
	class FolderFileIterator {
	public:
		bool Step();
		FolderEntry CurrentEntry();

		typedef std::shared_ptr<FolderFileIterator> Ptr;

		FolderFileIterator(std::string path);
	
	private:
		boost::filesystem::directory_iterator m_it;
	};
}

#endif
