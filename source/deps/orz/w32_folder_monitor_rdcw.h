#ifndef W32_FOLDER_MONITOR_RDCW_H
#define W32_FOLDER_MONITOR_RDCW_H

#include "folder_monitor_imp.h"
#include "folder.h"

namespace IO {
	class FolderMonitorWin32RDCW:public FolderMonitorImplementation {
	public:
		FolderMonitorWin32RDCW(const std::wstring& path, ChangeEventCallback& ev);
		~FolderMonitorWin32RDCW();

	private:
		static void threadWrapper(void* param);
		void ThreadMain();

		void ProcessNotification(PFILE_NOTIFY_INFORMATION currChange);

		std::wstring ExtractFilename(PFILE_NOTIFY_INFORMATION currChange);

		void FolderDeleted();

		enum {
			NumEvents = 4096
		};

		uint8_t m_changes[NumEvents * sizeof(FILE_NOTIFY_INFORMATION)];
		OVERLAPPED m_ol;

		HANDLE m_directory;
		std::shared_ptr<std::thread> m_thread;
		HANDLE m_ioComp;
		std::wstring m_oldName;
	};
}

#endif
