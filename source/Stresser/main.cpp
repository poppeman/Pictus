#include <conio.h>

#include "main.h"

#include "orz/fileops.h"
#include "illa/swsurface.h"
#include "illa/surfacemgr.h"

#include "orz/sysinfo.h"
#include "orz/folder.h"

#include "Helper.h"
#include "CacheUser.h"
#include "FolderUser.h"
#include "FolderManipulator.h"
#include "Logger.h"
#include "ThreadRunner.h"

#include <boost/locale.hpp>

ThreadRunner::Ptr g_runner;
bool g_run = true;

void prepareCacherTest(const std::string& path)
{
	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);
	g_runner->AddThread(BasicThread::Ptr(new CacheUser(path + "d\\", path + "e\\")));
}

void prepareFolderTest(const std::string& path)
{
	g_runner->AddThread(BasicThread::Ptr(new FolderUser(path + "d\\")));
	g_runner->AddThread(BasicThread::Ptr(new FolderManipulator(path + "d\\", path + "a\\")));
	g_runner->AddThread(BasicThread::Ptr(new FolderUser(path + "e\\")));
	g_runner->AddThread(BasicThread::Ptr(new FolderManipulator(path + "e\\", path + "b\\")));
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 3)
	{
		OutputString("Error: Bad params.\nStresser.exe CACHER|FOLDER|FULL PATH");
		return EXIT_FAILURE;
	}

	g_runner.reset(new ThreadRunner);

	auto test = boost::locale::to_upper(WStringToUTF8(argv[1]));

	wchar_t wfullPath[MAX_PATH];

	GetFullPathName(
		argv[2],
		MAX_PATH,
		wfullPath,
		0);

	auto fullPath = WStringToUTF8(wfullPath);

	auto path = IO::GetPath(fullPath + "\\");

	OutputString("Setting up ...");

	// FIXME: Cacher test didn't like it when these lines were missing
	copyFiles(path + "a\\", path + "d\\");
	copyFiles(path + "b\\", path + "e\\");

	auto numThreads = std::max<unsigned int>(2, std::thread::hardware_concurrency());

	OutputString("Processor count : " + ToAString(numThreads));

	if (test == "CACHER")
	{
		OutputString("Cacher-only test");
		prepareCacherTest(path);
	}
	else if (test == "FOLDER")
	{
		OutputString("Folder-only test");
		prepareFolderTest(path);
	}
	else if (test == "FULL")
	{
		OutputString("Full test");
		prepareCacherTest(path);
		prepareFolderTest(path);
	}

	OutputString("Starting threads, press Enter to quit ...");
	g_runner->Run();

	for (;;) {
		if(_kbhit()) {
			int c = _getch();
			if(c == '\n' || c=='\r')
				break;
		}
		if(!g_run)
			break;
		Sleep(100);
	}
	g_runner->Terminate();
	g_runner.reset();
	if(!g_run) {
		std::cout << "Some sort of error occurred. Press enter to terminate.\n";
		std::cin.get();
	}

	return EXIT_SUCCESS;
}

void SignalError(const std::string& err) {
	OutputString(err, true);
	g_runner->Stop();
	g_run = false;
}
