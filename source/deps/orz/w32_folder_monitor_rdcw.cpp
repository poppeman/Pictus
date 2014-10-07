#include "StdAfx.h"
#include "w32_folder_monitor_rdcw.h"
#include "folder_monitor.h"

#pragma comment(lib, "shlwapi.lib")

namespace IO {
	FolderMonitorImplementation* FolderMonitor::doCreateImp(const std::wstring& path, ChangeEventCallback& ev) {
		return new FolderMonitorWin32RDCW(path, ev);
	}

	FolderMonitorWin32RDCW::FolderMonitorWin32RDCW(const std::wstring& path, ChangeEventCallback& ev):
		FolderMonitorImplementation{ path, ev },
		m_directory{ 0 },
		m_ioComp{ 0 }
	{
		std::wstring cropped = path.substr(0, path.length()-1);
		ZeroMemory(&m_ol, sizeof(m_ol));

		m_directory = CreateFileW(
			cropped.c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			0,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			0);
		if (m_directory == 0 || m_directory == INVALID_HANDLE_VALUE) {
			DO_THROW(Err::InvalidParam, TX("Could not open directory:") + path);
		}

		m_ioComp = CreateIoCompletionPort(
			m_directory, 
			m_ioComp,
			reinterpret_cast<ULONG_PTR>(this),
			0);
		if (m_ioComp == 0) {
			DO_THROW(Err::CriticalError, TX("Could not create an IO completion port."));
		}

		if (m_thread.get()) {
			DO_THROW(Err::CriticalError, TX("Thread should NOT be running now!"));
		}
		m_thread = std::make_shared<std::thread>(&FolderMonitorWin32RDCW::threadWrapper, this);
	}

	void FolderMonitorWin32RDCW::threadWrapper(void* param) {
		reinterpret_cast<FolderMonitorWin32RDCW*>(param)->ThreadMain();
	}

	FileEventType DetermineAction(DWORD action) {
		if (action == FILE_ACTION_ADDED) {
			return FileAdded;
		}
		if (action == FILE_ACTION_REMOVED) {
			return FileRemoved;
		}
		if (action == FILE_ACTION_RENAMED_NEW_NAME) {
			return FileRenamed;
		}

		DO_THROW(Err::InvalidParam, L"Invalid action:" + ToWString(action));
	}

	void FolderMonitorWin32RDCW::ThreadMain() {
		while(true) {
			if(!ReadDirectoryChangesW(
				m_directory,
				m_changes,
				sizeof(m_changes),
				false,
				FILE_NOTIFY_CHANGE_FILE_NAME,
				0,
				&m_ol,
				0))
				break;

			DWORD numBytes;
			LPOVERLAPPED lpOverlapped = 0;
			ULONG_PTR key = 0;

			int ret=GetQueuedCompletionStatus(
				m_ioComp,
				&numBytes,
				&key,
				&lpOverlapped,
				100);

			if(!PathIsDirectoryW(Path().c_str())) {
				FolderDeleted();
				break;
			}

			bool timeout = (ret==0 && lpOverlapped==0);
			if(timeout)
				continue;

			if(numBytes == 0 || key == 0)
				break;

			int offset=0;
			PFILE_NOTIFY_INFORMATION currChange = 0;
			do {
				currChange = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(&m_changes[offset]);
				offset += currChange->NextEntryOffset;

				ProcessNotification(currChange);

			} while(currChange->NextEntryOffset);
		}
	}

	FolderMonitorWin32RDCW::~FolderMonitorWin32RDCW() {
		if(m_directory)
			CloseHandle(m_directory);
		if(m_ioComp)
			CloseHandle(m_ioComp);
		if(m_thread)
			m_thread->join();
	}

	void FolderMonitorWin32RDCW::ProcessNotification(PFILE_NOTIFY_INFORMATION currChange) {
		if(currChange->Action == FILE_ACTION_RENAMED_OLD_NAME) {
			m_oldName = ExtractFilename(currChange);
			return;
		}

		FileEvent e;
		e.Path = Path();
		e.Type = DetermineAction(currChange->Action);
		e.Entry.Name = ExtractFilename(currChange);
		e.Entry.Type = TypeFile;
		if(e.Type == FileRenamed)
			e.Entry.PreviousName = m_oldName;

		OnEvent(e);
	}

	void FolderMonitorWin32RDCW::FolderDeleted() {
		FileEvent e;
		e.Path = Path();
		e.Type = MonitoredFolderDeleted;
		e.Entry.Type = TypeFolder;
		OnEvent(e);
	}

	std::wstring FolderMonitorWin32RDCW::ExtractFilename(PFILE_NOTIFY_INFORMATION currChange) {
		wchar_t tmpstr[MAX_PATH];
		memcpy_s(tmpstr, MAX_PATH * sizeof(char), currChange->FileName, currChange->FileNameLength);
		tmpstr[currChange->FileNameLength / 2] = 0;
		return std::wstring(tmpstr);
	}
}
