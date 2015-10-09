#include "orz/folder_monitor.h"
#include "orz/fileops.h"
#include "main.h"
#include <UnitTest++/UnitTest++.h>

SUITE(TestFolderMonitor) {
	using namespace IO;
	TEST(NoStart) {
		FolderMonitor mon;
		mon.Close();
	}
	TEST(ManyStart) {
		FolderMonitor mon;
		mon.Start(g_datapath + L"\\Folder\\a\\");
		mon.Start(g_datapath + L"\\Folder\\a\\");
	}
	/*struct SimpleMonitor {
		void OnEvent(FileEvent e) {
			events.push_back(e);

		}
		SimpleMonitor() {
			mon.OnEvent.connect(bind(&SimpleMonitor::OnEvent, this, _1));
		}
		FolderMonitor mon;
		std::vector<FileEvent> events;
	};
	TEST_FIXTURE(SimpleMonitor, DetectAddDelete) {
		mon.Start(g_datapath + L"\\Folder\\a\\");
		Sleep(100);
		CHECK(CopyFile(
			(g_datapath + L"\\Folder\\a\\file1.txt").c_str(),
			(g_datapath + L"\\Folder\\a\\temp.txt").c_str(),
			false));
		CHECK(DeleteFile((g_datapath + L"\\Folder\\a\\temp.txt").c_str()));
		Sleep(100);
		mon.Close();
		CHECK_EQUAL(2, events.size());
		if(events.size() < 2) return;
		for(int i=0;i<2;++i) {
			CHECK(L"temp.txt" == events[i].Entry.Name);
			CHECK_EQUAL(TypeFile, events[i].Entry.Type);
			CHECK(g_datapath + L"\\Folder\\a\\" == events[i].Path);
		}
		CHECK_EQUAL(FileAdded, events[0].Type);
		CHECK_EQUAL(FileRemoved, events[1].Type);
	}*/
} 
