#ifndef ORZ_LINUX_FOLDER_MONITOR_H
#define ORZ_LINUX_FOLDER_MONITOR_H

#include "../folder_monitor_imp.h"
#include "../folder.h"

namespace IO {
	class FolderMonitorLinux:public FolderMonitorImplementation {
	public:
		FolderMonitorLinux(const std::string& path, ChangeEventCallback& ev);
		~FolderMonitorLinux();
	};
}

#endif
