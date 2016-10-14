#include "fileops.h"
#include "file_reader.h"

#include <boost/filesystem.hpp>

namespace IO {
	bool DoFileExist(const std::string& file) {
		if (file.empty()) {
			return false;
		}

		IO::FileReader r(file);
		return r.Open();
	}

	bool DoPathExist(const std::string& file) {
		return boost::filesystem::is_directory(file);
	}

	std::string GetFile(const std::string& s) {
		return boost::filesystem::path(s).filename().string();
	}

	std::string GetTitle(const std::string& s) {
		return boost::filesystem::path(s).stem().string();
	}

	std::string GetExtension(const std::string& s) {
		auto ext = boost::filesystem::path(s).extension().string();
		if (ext.length() > 0 && ext[0] == '.') {
			return ext.substr(1);
		}
		return ext;
	}
}
