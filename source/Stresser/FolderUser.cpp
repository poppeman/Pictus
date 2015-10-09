#include "orz/folder.h"
#include "orz/file_reader.h"
#include "FolderUser.h"


FolderUser::FolderUser(const std::wstring& folder)
	:m_folder(folder)
{}

FolderUser::~FolderUser() {}

void FolderUser::ThreadMain()
{
	Output(L"Initializing FolderUser, scanning " + m_folder + L" ...");
	IO::Folder folder;

	folder.Path(m_folder);

	Output(L"Init complete, running ...");

	auto files = folder.CurrentContents();
	auto i = files.begin();

	while(IsTerminating() == false)
	{
		i++;
		if (i != files.end())
		{
			if (i->Type == IO::TypeFile)
			{
				std::wstring name = m_folder + i->Name;

				IO::FileReader file(name);
				if (file.Open())
				{
					uint8_t buf[4096];
					file.Read(buf, 1, 4096);
				}
			}
		}
		else
		{
			files = folder.CurrentContents();
			i = files.begin();
		}
	}
}
