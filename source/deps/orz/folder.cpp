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

		for (auto x : iter) {
			FolderEntry toRet;
			if (boost::filesystem::is_directory(x)) {
				toRet.Type = TypeFolder;
			}
			if (boost::filesystem::is_regular_file(x)) {
				toRet.Type = TypeFile;
			}

			toRet.Name = x.path().filename().string();
			files.push_back(toRet);
		}
		return files;
	}

	boost::filesystem::directory_iterator  Folder::CreateIterator() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		return boost::filesystem::directory_iterator(m_path);
	}
}
