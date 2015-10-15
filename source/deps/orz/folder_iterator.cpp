#include "folder_iterator.h"

namespace IO {
	bool FolderFileIterator::Step() {
		m_it++;
		return m_it != boost::filesystem::directory_iterator();
	}

	IO::FolderEntry FolderFileIterator::CurrentEntry() {
		FolderEntry toRet;

		if(boost::filesystem::is_directory(*m_it)) {
			toRet.Type = TypeFolder;
		}
		if(boost::filesystem::is_regular_file(*m_it)) {
			toRet.Type = TypeFile;
		}

		toRet.Name = m_it->path().filename().string();

		return toRet;
		
	}

	FolderFileIterator::FolderFileIterator(std::string path) {
		boost::filesystem::path p(path);
		m_it = boost::filesystem::directory_iterator(p);
	}
}
