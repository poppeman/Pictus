#include "folder_monitor_imp.h"

namespace IO {
	FolderMonitorImplementation::FolderMonitorImplementation(const std::string& path, ChangeEventCallback& c):
		OnEvent(c),
		m_path(path)
	{}

	const std::string& FolderMonitorImplementation::Path() {
		return m_path;
	}

	FolderMonitorImplementation::~FolderMonitorImplementation() {}
}
