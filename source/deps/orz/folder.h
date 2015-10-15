#ifndef ORZ_FOLDER_H
#define ORZ_FOLDER_H

#include "folder_types.h"
#include <mutex>
#include <boost/filesystem.hpp>

namespace IO {
	class Folder:boost::noncopyable {
	public:
		bool Path(const std::string& filename);
		std::string Path() const;

		boost::filesystem::directory_iterator CreateIterator() const;
		IO::FileList CurrentContents() const;

		Folder();

	private:
		mutable std::mutex m_mxCall;
		FileList m_content;
		std::string m_path;
	};
}

#endif
