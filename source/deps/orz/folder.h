#ifndef FOLDER_H
#define FOLDER_H

#include "folder_imp.h"
#include "folder_iterator.h"

namespace IO {
	class Folder:boost::noncopyable {
	public:
		bool Path(const std::wstring& filename);
		std::wstring Path() const;

		FolderFileIterator::Ptr CreateIterator() const;
		IO::FileList CurrentContents() const;

		Folder();

	private:
		FolderImplementation* doCreateImp(const std::wstring& path);

	private:
		mutable std::mutex m_mxCall;
		FileList m_content;

		std::auto_ptr<FolderImplementation>	m_imp;
	};
}

#endif
