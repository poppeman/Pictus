
#include "StdAfx.h"

#include "orz/folder.h"
#include "orz/fileops.h"
#include "orz/orz_math.h"

#include "FolderManipulator.h"
#include "Helper.h"

FolderManipulator::FolderManipulator(const std::wstring& folder, const std::wstring& sourceFolder)
	:m_folder(folder),
	 m_sourceFolder(sourceFolder)
{}

FolderManipulator::~FolderManipulator() {}

void FolderManipulator::ThreadMain()
{
	Output(TX("Initializing FolderManipulator, scanning ") + m_folder + TX(" ..."));

	IO::Folder f;
	f.Path(m_folder);

	IO::FileList files = f.CurrentContents();

	Output(TX("Iterating ..."));

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
			Output(TX("Refilling folder!"));
			copyFiles(m_sourceFolder, m_folder);
		}
	}

	Output(TX("Init complete, running ..."));
	auto currentFile = files.begin();

	while(IsTerminating() == false)
	{
		Math::Randomizer random;
		int action = random.Random() % 10;

		std::wstring nameCurrentFile = m_folder + currentFile->Name;
		std::wstring nameNewFile = m_folder + TX("z") + currentFile->Name;

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
							Output(TX("Out of files, bailing!"));
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
