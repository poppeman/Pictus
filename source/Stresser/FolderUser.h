
#ifndef FOLDERUSER_H
#define FOLDERUSER_H

#include "BasicThread.h"

class FolderUser:public BasicThread
{
public:
	FolderUser(const std::wstring& folder);
	~FolderUser();

	typedef std::shared_ptr<FolderUser> Ptr;

private:
	void							ThreadMain();

private:
	std::wstring							m_folder;
};

#endif
