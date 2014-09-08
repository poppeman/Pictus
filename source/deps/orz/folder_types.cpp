#include "StdAfx.h"
#include "folder_types.h"

namespace IO {
	bool operator==(const FileEvent& lhs, const FileEvent& rhs) {
		return (lhs.Entry == rhs.Entry) && (lhs.Type == rhs.Type);
	}

	bool operator ==(const FolderEntry& a, const FolderEntry& b) {
		return (a.Name == b.Name) && (a.Type == b.Type);
	}

}
