#ifndef FOLDER_TYPES_H
#define FOLDER_TYPES_H

namespace IO {
	enum FileEventType {
		FileAdded,
		FileRemoved,
		FileRenamed,
		MonitoredFolderDeleted,
	};

	enum FolderEntryType {
		TypeDotFolder,
		TypeFolder,
		TypeFile
	};

	struct FolderEntry {
		std::wstring Name;
		FolderEntryType Type;
		std::wstring PreviousName;
	};

	struct FileEvent {
		std::wstring Path;
		FolderEntry Entry;
		FileEventType Type;
	};

	typedef std::list<FileEvent> FileEventList;

	bool operator==(const FileEvent& lhs, const FileEvent& rhs);
	bool operator ==(const FolderEntry& a, const FolderEntry& b);

	typedef std::list<FolderEntry>	FileList;

	typedef boost::signals2::signal<void(IO::FileEvent)> ChangeEventCallback;
}

#endif
