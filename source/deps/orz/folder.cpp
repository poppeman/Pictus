#include "folder.h"
#include "exception.h"
#include "fileops.h"
#include "types.h"

namespace IO {
	Folder::Folder() {}

	bool Folder::Path(const std::string& path) {
		std::lock_guard<std::mutex> l(m_mxCall);

		if (!IO::DoPathExist(path)) {
			return false;
		}
		m_path = path;

		return true;
	}

	std::string Folder::Path() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		return m_path;
	}

	FileList Folder::CurrentContents() const {
		auto iter = CreateIterator();
		FileList files;
		files.push_back(iter->CurrentEntry());
		while (iter->Step()) {
			files.push_back(iter->CurrentEntry());
		}
		return files;
	}

	FolderFileIterator::Ptr Folder::CreateIterator() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		return std::make_shared<FolderFileIterator>(m_path);
	}
}
