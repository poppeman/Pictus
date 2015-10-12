#ifndef ORZ_W32_FOLDER_ITERATOR_H
#define ORZ_W32_FOLDER_ITERATOR_H

#include "../folder_iterator.h"
#include <windows.h>

namespace IO {
	class FolderFileIteratorWin32:public FolderFileIterator {
	public:
		FolderFileIteratorWin32(const std::string& path);
		~FolderFileIteratorWin32();

	private:
		bool OnStep();
		FolderEntry OnCurrentEntry();

		void FindNext();

		bool m_first;
		WIN32_FIND_DATAW m_currentEntry;

		HANDLE m_handle;
	};
}

#endif
