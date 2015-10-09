#include "w32_folder_iterator.h"
#include "exception.h"
#include "types.h"

namespace IO {
	bool FolderFileIteratorWin32::OnStep() {
		if(m_first) {
			m_first = false;
			return true;
		}

		return(FindNextFileW(m_handle, &m_currentEntry) != 0);
	}

	FolderEntry FolderFileIteratorWin32::OnCurrentEntry() {
		FolderEntryType type(TypeFile);
		std::wstring name(m_currentEntry.cFileName);

		if (name == L"." || name == L"..") {
			type = TypeDotFolder;
		}
		else if (m_currentEntry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			type = TypeFolder;
		}

		FolderEntry currentEntry = {m_currentEntry.cFileName, type};
		return currentEntry;
	}

	FolderFileIteratorWin32::FolderFileIteratorWin32(const std::wstring& path):m_handle(0), m_first(true) {
		m_handle = FindFirstFileW((path + L"*.*").c_str(), &m_currentEntry);
		if (m_handle == INVALID_HANDLE_VALUE) {
			DO_THROW(Err::InvalidParam, L"Call to FindFirstFile failed. Path = " + path);
		}
	}

	FolderFileIteratorWin32::~FolderFileIteratorWin32() {
		if (m_handle) {
			FindClose(m_handle);
		}
	}
}
