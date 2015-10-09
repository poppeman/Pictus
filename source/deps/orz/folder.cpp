#include "folder.h"
#include "exception.h"
#include "fileops.h"
#include "types.h"

namespace IO {
	Folder::Folder() {}

	bool Folder::Path(const std::wstring& path) {
		std::lock_guard<std::mutex> l(m_mxCall);

		if (!IO::DoPathExist(path)) {
			return false;
		}

		m_imp.reset(doCreateImp(path));
		return true;
	}

	std::wstring Folder::Path() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		if (m_imp.get() == 0) {
			return L"";
		}
		return m_imp->Path();
	}

	FileList Folder::CurrentContents() const {
		FolderFileIterator::Ptr iter(CreateIterator());
		FileList files;
		while (iter->Step()) {
			files.push_back(iter->CurrentEntry());
		}
		return files;
	}

	FolderFileIterator::Ptr Folder::CreateIterator() const {
		std::lock_guard<std::mutex> l(m_mxCall);
		if (m_imp.get() == 0) {
			DO_THROW(Err::InvalidCall, "Implementation not set.");
		}
		return m_imp->CreateIterator();
	}
}
