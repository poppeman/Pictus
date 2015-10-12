#include "orz/folder.h"
#include "main.h"
#include <UnitTest++/UnitTest++.h>

SUITE(FolderTest)
{
	TEST(UnusedFolder)
	{
		IO::Folder folder;
		CHECK(true);
	}

	struct NonFiles {
		bool operator()(const IO::FolderEntry& e) {
			if(e.Type != IO::TypeFile)
				return true;
			return false;
		}
	};

	TEST(MonitorScanNoEndSlash)
	{
		IO::Folder folder;
		folder.Path(g_datapath + "\\Folder\\a");
		IO::FileList l = folder.CurrentContents();
		l.remove_if(NonFiles());
		CHECK_EQUAL(2, l.size());
		if(l.size() == 2) {
			CHECK(l.front().Name == "file1.txt");
			CHECK(l.back().Name == "file2.txt");
		}
	}

	TEST(MonitorScanEndSlash)
	{
		IO::Folder folder;
		folder.Path(g_datapath + "\\Folder\\a\\");
		IO::FileList l = folder.CurrentContents();
		l.remove_if(NonFiles());
		CHECK_EQUAL(2, l.size());
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
