#include "w32_folder.h"
#include "w32_folder_iterator.h"
#include "../folder.h"

namespace IO {
	FolderImplementation* Folder::doCreateImp(const std::string& path) {
		return new FolderWin32(path);
	}

	FolderWin32::FolderWin32(const std::string& path):FolderImplementation(SetFolderSlashes(path)) {}

	FolderFileIterator::Ptr FolderWin32::CreateIterator() {
		return std::make_shared<FolderFileIteratorWin32>(Path());
	}

	std::string FolderWin32::SetFolderSlashes(const std::string& inPath) {
		char trm = *inPath.rbegin();
		if(trm != '\\' && trm != '/') {
			return inPath + "\\";
		}
		return inPath;
	}
}
