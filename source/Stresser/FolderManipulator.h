
#ifndef STRESSER_FOLDERMANIPULATOR_H
#define STRESSER_FOLDERMANIPULATOR_H

#include "BasicThread.h"

class FolderManipulator:public BasicThread
{
public:
	FolderManipulator(const std::string& folder, const std::string& sourceFolder);
	~FolderManipulator();

private:
	void ThreadMain();

private:
	std::string m_folder;
	std::string m_sourceFolder;
};


#endif;