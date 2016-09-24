#include "linux_folder_monitor.h"
#include "../folder_monitor.h"

namespace IO
{
	FolderMonitorImplementation* FolderMonitor::doCreateImp(const std::string& path, ChangeEventCallback& ev) {
		return new FolderMonitorLinux(path, ev);
	}

	FolderMonitorLinux::FolderMonitorLinux(const std::string &path, ChangeEventCallback &ev):
		FolderMonitorImplementation{ path, ev }
	{

	}

	FolderMonitorLinux::~FolderMonitorLinux()
	{

	}
}

