#include "w32_folder.h"
#include "w32_folder_iterator.h"
#include "folder.h"
#include "fileops.h"

namespace IO {
	bool Win32Folder::Path(const std::string& path) {
		std::lock_guard<std::mutex> l(m_mxCall);

		if (!IO::DoPathExist(path)) {
			return false;
		}
		m_path = path;

		return true;
	}

	std::string Win32Folder::Path() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		return m_path;
	}

	std::shared_ptr<FolderFileIteratorWin32> Win32Folder::CreateIterator() const {
		return std::make_shared<FolderFileIteratorWin32>(Path());
	}

	FileList Win32Folder::CurrentContents() const {
		auto iter(CreateIterator());
		FileList files;
		while (iter->Step()) {
			files.push_back(iter->CurrentEntry());
		}
		return files;
	}
}
