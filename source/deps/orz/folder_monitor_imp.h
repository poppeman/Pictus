#ifndef ORZ_FOLDER_MONITOR_IMP_H
#define ORZ_FOLDER_MONITOR_IMP_H

#include "folder_types.h"

namespace IO {
	class FolderMonitorImplementation:boost::noncopyable {
	public:
		ChangeEventCallback& OnEvent;

		FolderMonitorImplementation(const std::string& path, ChangeEventCallback& ev);
		virtual ~FolderMonitorImplementation();

	protected:
		const std::string& Path();

	private:
		std::string m_path;
	};
}

#endif
