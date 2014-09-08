#include "StdAfx.h"
#include "w32_folder.h"
#include "w32_folder_iterator.h"
#include "folder.h"

namespace IO {
	FolderImplementation* Folder::doCreateImp(const std::wstring& path) {
		return new FolderWin32(path);
	}

	FolderWin32::FolderWin32(const std::wstring& path):FolderImplementation(SetFolderSlashes(path)) {}

	FolderFileIterator::Ptr FolderWin32::CreateIterator() {
		return std::make_shared<FolderFileIteratorWin32>(Path());
	}

	std::wstring FolderWin32::SetFolderSlashes(const std::wstring& inPath) {
		char trm = *inPath.rbegin();
		if(trm != TX('\\') && trm != TX('/'))
			return inPath + TX("\\");
		return inPath;
	}
}
