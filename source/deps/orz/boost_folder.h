#ifndef ORZ_BOOST_FOLDER_H
#define ORZ_BOOST_FOLDER_H

#include "folder_types.h"
#include <mutex>
#include <boost/filesystem.hpp>

namespace IO {
	class BoostFolder:boost::noncopyable {
	public:
		bool Path(const std::string& filename);
		std::string Path() const;

		boost::filesystem::directory_iterator CreateIterator() const;
		IO::FileList CurrentContents() const;

	private:
		mutable std::mutex m_mxCall;
		FileList m_content;
		std::string m_path;
	};

	typedef BoostFolder Folder;
}

#endif
