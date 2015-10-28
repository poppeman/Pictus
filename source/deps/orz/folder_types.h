#ifndef ORZ_FOLDER_TYPES_H
#define ORZ_FOLDER_TYPES_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4512)
#endif
#include <boost/signals2.hpp>
#include <list>

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
		std::string Name;
		FolderEntryType Type;
		std::string PreviousName;
	};

	struct FileEvent {
		std::string Path;
		FolderEntry Entry;
		FileEventType Type;
	};

	typedef std::list<FileEvent> FileEventList;

	bool operator==(const FileEvent& lhs, const FileEvent& rhs);
	bool operator ==(const FolderEntry& a, const FolderEntry& b);

	typedef std::vector<FolderEntry>	FileList;

	typedef boost::signals2::signal<void(IO::FileEvent)> ChangeEventCallback;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
