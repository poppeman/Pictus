#ifndef ORZ_W32_FOLDER_MONITOR_RDCW_H
#define ORZ_W32_FOLDER_MONITOR_RDCW_H

#include "../folder_monitor_imp.h"
#include "../folder.h"

#include <thread>

#include <windows.h>

namespace IO {
	class FolderMonitorWin32RDCW:public FolderMonitorImplementation {
	public:
		FolderMonitorWin32RDCW(const std::string& path, ChangeEventCallback& ev);
		~FolderMonitorWin32RDCW();

	private:
		static void threadWrapper(void* param);
		void ThreadMain();

		void ProcessNotification(PFILE_NOTIFY_INFORMATION currChange);

		std::string ExtractFilename(PFILE_NOTIFY_INFORMATION currChange);

		void FolderDeleted();

		enum {
			NumEvents = 4096
		};

		uint8_t m_changes[NumEvents * sizeof(FILE_NOTIFY_INFORMATION)] = {};
		OVERLAPPED m_ol = {};

		HANDLE m_directory = nullptr;
		std::shared_ptr<std::thread> m_thread;
		HANDLE m_ioComp = nullptr;
		std::string m_oldName;
	};
}

#endif
