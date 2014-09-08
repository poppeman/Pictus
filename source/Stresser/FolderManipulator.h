

#ifndef FOLDERMANIPULATOR_H
#define FOLDERMANIPULATOR_H

#include "BasicThread.h"

class FolderManipulator:public BasicThread
{
public:
	FolderManipulator(const std::wstring& folder, const std::wstring& sourceFolder);
	~FolderManipulator();

private:
	void							ThreadMain();

private:
	std::wstring							m_folder;
	std::wstring							m_sourceFolder;
};


#endif;