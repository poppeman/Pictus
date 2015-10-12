#ifndef ORZ_W32_FOLDER_H
#define ORZ_W32_FOLDER_H

#include "../folder_imp.h"

namespace IO {
	class FolderWin32:public FolderImplementation {
	public:
		FolderFileIterator::Ptr CreateIterator();

		explicit FolderWin32(const std::string& path);

	private:
		std::string SetFolderSlashes(const std::string& inPath);
	};
}

#endif
