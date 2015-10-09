#include "orz/folder.h"
#include "orz/fileops.h"
#include "orz/orz_math.h"

#include "FolderManipulator.h"
#include "Helper.h"

#include <boost/random.hpp>

FolderManipulator::FolderManipulator(const std::wstring& folder, const std::wstring& sourceFolder)
	:m_folder(folder),
	 m_sourceFolder(sourceFolder)
{}

FolderManipulator::~FolderManipulator() {}

void FolderManipulator::ThreadMain()
{
	Output(L"Initializing FolderManipulator, scanning " + m_folder + L" ...");

	IO::Folder f;
	f.Path(m_folder);

	IO::FileList files = f.CurrentContents();

	Output(L"Iterating ...");

	// Avoid variable hiding ugliness
	{
		bool found = false;
		for (const auto& i : files)
		{
			if (i.Type == IO::TypeFile)
			{
				found = true;
				break;
			}
		}

		if (found == false)
		{
			Output(L"Refilling folder!");
			copyFiles(m_sourceFolder, m_folder);
		}
	}

	Output(L"Init complete, running ...");
	auto currentFile = files.begin();

	boost::random::mt19937 r;
	boost::random::uniform_int_distribution<> act(0, 10);

	while (IsTerminating() == false)
	{
		int action = act(r);

		std::wstring nameCurrentFile = m_folder + currentFile->Name;
		std::wstring nameNewFile = m_folder + L"z" + currentFile->Name;

		switch (action)
		{
		case 0:
			if (currentFile->Type == IO::TypeFile)
			{
				IO::Rename(nameCurrentFile, nameNewFile, 0);
			}
			break;
		case 1:
			if (currentFile->Type == IO::TypeFile)
			{
				MoveFileW(nameCurrentFile.c_str(), nameNewFile.c_str());
			}
			break;
		case 2:
			if (currentFile->Type == IO::TypeFile)
			{
				IO::FileDelete(nameCurrentFile.c_str(), 0);
			}
			break;
		default:
			{
				bool found = false;
				bool restarted = false;
				while (!found)
				{
					currentFile++;
					if (currentFile == files.end())
					{
						if (restarted == false)
						{
							files = f.CurrentContents();
							currentFile = files.begin();
							restarted = true;
						}
						else
						{
							Output(L"Out of files, bailing!");
							return;
						}
					}


					if (currentFile->Type == IO::TypeFile)
					{
						found = true;
						break;
					}
				};
			}
			break;
		}
	}
}
