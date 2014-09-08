#include "StdAfx.h"
#include "folder_monitor_imp.h"

namespace IO {
	FolderMonitorImplementation::FolderMonitorImplementation(const std::wstring& path, ChangeEventCallback& c):m_path(path), OnEvent(c) {
	}

	const std::wstring& FolderMonitorImplementation::Path() {
		return m_path;
	}

	FolderMonitorImplementation::~FolderMonitorImplementation() {}
}
