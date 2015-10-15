#include "orz/folder.h"
#include "main.h"
#include <UnitTest++/UnitTest++.h>

void SortFiles(IO::FileList& l) {
	std::sort(l.begin(), l.end(), [](const IO::FolderEntry& lhs, const IO::FolderEntry& rhs) { return lhs.Name < rhs.Name; });
}

SUITE(FolderTest)
{
	TEST(UnusedFolder)
	{
		IO::Folder folder;
		CHECK(true);
	}

	TEST(MonitorScanNoEndSlash)
	{
		IO::Folder folder;
		folder.Path(g_datapath + "/Folder/a");
		IO::FileList l = folder.CurrentContents();

		CHECK_EQUAL(2, l.size());
		SortFiles(l);

		if(l.size() == 2) {
			CHECK(l.front().Name == "file1.txt");
			CHECK(l.back().Name == "file2.txt");
		}
	}

	TEST(MonitorScanEndSlash)
	{
		IO::Folder folder;
		folder.Path(g_datapath + "/Folder/a/");
		IO::FileList l = folder.CurrentContents();
		CHECK_EQUAL(2, l.size());
		SortFiles(l);

		if(l.size() == 2) {
			CHECK(l.front().Name == "file1.txt");
			CHECK(l.back().Name =="file2.txt");
		}
	}


	TEST(InvalidPaths)
	{
		IO::Folder folder;
		CHECK(!folder.Path("C:\\folder_that_does_not_exist"));
		CHECK(!folder.Path("C:\\folder_that_does_not_exist\\file.jpg"));
	}
}
