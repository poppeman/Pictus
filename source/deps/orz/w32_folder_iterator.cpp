#include "w32_folder_iterator.h"
#include "types.h"
#include "exception.h"

namespace IO {
	std::wstring SetFolderSlashes(const std::wstring& inPath) {
		auto trm = *inPath.rbegin();
		if (trm != L'\\' && trm != L'/') {
			return inPath + L"\\";
		}
		return inPath;
	}

	bool FolderFileIteratorWin32::Step() {
		if (m_first) {
			m_first = false;
			return true;
		}

		return(FindNextFileW(m_handle, &m_currentEntry) != 0);
	}

	FolderEntry FolderFileIteratorWin32::CurrentEntry() {
		FolderEntryType type(TypeFile);
		std::wstring name(m_currentEntry.cFileName);

		if (name == L"." || name == L"..") {
			type = TypeDotFolder;
		}
		else if (m_currentEntry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			type = TypeFolder;
		}

		FolderEntry currentEntry = { WStringToUTF8(m_currentEntry.cFileName), type };
		return currentEntry;
	}

	FolderFileIteratorWin32::FolderFileIteratorWin32(const std::string& path) :m_handle(0), m_first(true) {
		auto wpath = SetFolderSlashes(UTF8ToWString(path.c_str()));

		m_handle = FindFirstFileW((wpath + L"*.*").c_str(), &m_currentEntry);
		if (m_handle == INVALID_HANDLE_VALUE) {
			DO_THROW(Err::InvalidParam, "Call to FindFirstFile failed. Path = " + path);
		}
	}

	FolderFileIteratorWin32::~FolderFileIteratorWin32() {
		if (m_handle) {
			FindClose(m_handle);
		}
	}
}
