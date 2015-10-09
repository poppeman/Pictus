#include "folder_iterator.h"

namespace IO {
	bool FolderFileIterator::Step() {
		return OnStep();
	}

	IO::FolderEntry FolderFileIterator::CurrentEntry() {
		return OnCurrentEntry();
	}

	FolderFileIterator::~FolderFileIterator() {
	}
}
