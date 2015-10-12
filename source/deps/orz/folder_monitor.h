#ifndef ORZ_FOLDER_MONITOR_H
#define ORZ_FOLDER_MONITOR_H

#include "folder_types.h"
#include "folder_monitor_imp.h"
#include "folder.h"

namespace IO {
	class FolderMonitor:boost::noncopyable {
	public:
		ChangeEventCallback OnEvent;

		void Start(const std::string& path);
		void Close();

		~FolderMonitor();

	private:
		FolderMonitorImplementation* doCreateImp(const std::string& path, ChangeEventCallback& c);

		std::auto_ptr<FolderMonitorImplementation> m_imp;
	};
}

#endif
