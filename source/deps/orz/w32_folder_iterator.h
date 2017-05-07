#ifndef ORZ_W32_FOLDER_ITERATOR_H
#define ORZ_W32_FOLDER_ITERATOR_H

#include <windows.h>
#include <wx/msw/winundef.h>
#include "folder_types.h"

namespace IO {
	class FolderFileIteratorWin32 {
	public:
		bool Step();
		FolderEntry CurrentEntry();

		FolderFileIteratorWin32(const std::string& path);
		~FolderFileIteratorWin32();

	private:
		void FindNext();

		bool m_first;
		WIN32_FIND_DATAW m_currentEntry;

		HANDLE m_handle;
	};
}

#endif
