#ifndef FOLDER_MONITOR_IMP_H
#define FOLDER_MONITOR_IMP_H

#include "folder_types.h"

namespace IO {
	class FolderMonitorImplementation:boost::noncopyable {
	public:
		ChangeEventCallback& OnEvent;

		FolderMonitorImplementation(const std::wstring& path, ChangeEventCallback& ev);
		virtual ~FolderMonitorImplementation();

	protected:
		const std::wstring& Path();

	private:
		std::wstring m_path;
	};
}

#endif
