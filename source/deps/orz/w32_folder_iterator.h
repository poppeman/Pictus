#ifndef W32_FOLDER_ITERATOR_H
#define W32_FOLDER_ITERATOR_H

#include "folder_iterator.h"

namespace IO {
	class FolderFileIteratorWin32:public FolderFileIterator {
	public:
		FolderFileIteratorWin32(const std::wstring& path);
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
