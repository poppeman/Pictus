#ifndef FOLDER_IMP_H
#define FOLDER_IMP_H

#include "folder_iterator.h"

namespace IO {
	class FolderImplementation:boost::noncopyable {
	public:
		const std::wstring& Path() const;
		virtual FolderFileIterator::Ptr CreateIterator()=0;

		FolderImplementation(const std::wstring& path);

	private:
		std::wstring m_path;
	};
}

#endif
