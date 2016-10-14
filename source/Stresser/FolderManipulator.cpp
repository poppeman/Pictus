#include "orz/folder.h"
#include "orz/fileops.h"
#include "orz/orz_math.h"
#include "orz/types.h"

#include "FolderManipulator.h"
#include "Helper.h"

#include <random>
#include <windows.h>

FolderManipulator::FolderManipulator(const std::string& folder, const std::string& sourceFolder):
	m_folder(folder),
	m_sourceFolder(sourceFolder)
{}

FolderManipulator::~FolderManipulator() {}

void FolderManipulator::ThreadMain()
{
	Output("Initializing FolderManipulator, scanning " + m_folder + " ...");

	IO::Folder f;
	f.Path(m_folder);

	IO::FileList files = f.CurrentContents();

	Output("Iterating ...");

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
			Output("Refilling folder!");
			copyFiles(m_sourceFolder, m_folder);
		}
	}

	Output("Init complete, running ...");
	auto currentFile = files.begin();

	std::random_device rd;
	std::default_random_engine r(rd());
	std::uniform_int_distribution<> act(0, 10);

	while (IsTerminating() == false)
	{
		int action = act(r);

		auto nameCurrentFile = m_folder + currentFile->Name;
		auto nameNewFile = m_folder + "z" + currentFile->Name;

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
				MoveFileW(UTF8ToWString(nameCurrentFile).c_str(), UTF8ToWString(nameNewFile).c_str());
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
							Output("Out of files, bailing!");
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
