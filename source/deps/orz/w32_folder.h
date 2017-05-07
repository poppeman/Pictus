#ifndef ORZ_W32_FOLDER_H
#define ORZ_W32_FOLDER_H

#include "folder_types.h"
#include "w32_folder_iterator.h"
#include <mutex>

namespace IO {
	class Win32Folder :boost::noncopyable {
	public:
		bool Path(const std::string& filename);
		std::string Path() const;

		std::shared_ptr<FolderFileIteratorWin32> CreateIterator() const;
		IO::FileList CurrentContents() const;

	private:
		mutable std::mutex m_mxCall;
		FileList m_content;
		std::string m_path;
	};

	typedef Win32Folder Folder;
}

#endif
