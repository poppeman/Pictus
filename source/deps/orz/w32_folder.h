#ifndef W32_FOLDER_H
#define W32_FOLDER_H

#include "folder_imp.h"

namespace IO {
	class FolderWin32:public FolderImplementation {
	public:
		FolderFileIterator::Ptr CreateIterator();

		explicit FolderWin32(const std::wstring& path);

	private:
		std::wstring SetFolderSlashes(const std::wstring& inPath);
	};
}

#endif
