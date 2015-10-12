#include "folder_monitor_imp.h"

namespace IO {
	FolderMonitorImplementation::FolderMonitorImplementation(const std::string& path, ChangeEventCallback& c):
		m_path(path),
		OnEvent(c)
	{}

	const std::string& FolderMonitorImplementation::Path() {
		return m_path;
	}

	FolderMonitorImplementation::~FolderMonitorImplementation() {}
}
