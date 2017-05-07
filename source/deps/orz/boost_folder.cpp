#include "boost_folder.h"
#include "exception.h"
#include "fileops.h"
#include "types.h"
#include <iterator>

namespace IO {
	bool BoostFolder::Path(const std::string& path) {
		std::lock_guard<std::mutex> l(m_mxCall);

		if (!IO::DoPathExist(path)) {
			return false;
		}
		m_path = path;

		return true;
	}

	std::string BoostFolder::Path() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		return m_path;
	}

	FileList BoostFolder::CurrentContents() const {
		auto iter = CreateIterator();
		FileList files;

		for (auto x = iter; x != boost::filesystem::directory_iterator(); x++) {
			try
			{
				FolderEntry toRet;

				if (boost::filesystem::is_directory(*x)) {
					toRet.Type = TypeFolder;
				}
				if (boost::filesystem::is_regular_file(*x)) {
					toRet.Type = TypeFile;
				}

				toRet.Name = x->path().filename().string();
				files.push_back(toRet);
			}
			catch (boost::filesystem::filesystem_error& err)
			{
				// Boost can throw exceptions in is_directory and probably is_regular_file for various reasons that are
				// entirely uninteresting to us. Try to do the best of the situation and ignore files/folders that
				// are causing issues.
			}
		}
		return files;
	}

	boost::filesystem::directory_iterator  BoostFolder::CreateIterator() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		return boost::filesystem::directory_iterator(m_path);
	}
}
