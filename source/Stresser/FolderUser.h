#ifndef STRESSER_FOLDERUSER_H
#define STRESSER_FOLDERUSER_H

#include "BasicThread.h"

class FolderUser:public BasicThread
{
public:
	FolderUser(const std::string& folder);
	~FolderUser();

	typedef std::shared_ptr<FolderUser> Ptr;

private:
	void ThreadMain();

private:
	std::string m_folder;
};

#endif
