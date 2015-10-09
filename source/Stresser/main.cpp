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

ThreadRunner::Ptr g_runner;
bool g_run = true;

void prepareCacherTest(const std::wstring& path)
{
	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);
	g_runner->AddThread(BasicThread::Ptr(new CacheUser(path + L"d\\", path + L"e\\")));
}

void prepareFolderTest(const std::wstring& path)
{
	g_runner->AddThread(BasicThread::Ptr(new FolderUser(path + L"d\\")));
	g_runner->AddThread(BasicThread::Ptr(new FolderManipulator(path + L"d\\", path + L"a\\")));
	g_runner->AddThread(BasicThread::Ptr(new FolderUser(path + L"e\\")));
	g_runner->AddThread(BasicThread::Ptr(new FolderManipulator(path + L"e\\", path + L"b\\")));
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc < 3)
	{
		OutputString(L"Error: Bad params.\nStresser.exe CACHER|FOLDER|FULL PATH");
		return EXIT_FAILURE;
	}

	g_runner.reset(new ThreadRunner);

	std::wstring test = ToUpper(argv[1]);

	wchar_t fullPath[MAX_PATH];

	GetFullPathName(
		argv[2],
		MAX_PATH,
		fullPath,
		0);

	std::wstring path = IO::GetPath(fullPath + std::wstring(L"\\"));

	OutputString(L"Setting up ...");

	// FIXME: Cacher test didn't like it when these lines were missing
	copyFiles(path + L"a\\", path + L"d\\");
	copyFiles(path + L"b\\", path + L"e\\");

	auto numThreads = std::max<unsigned int>(2, std::thread::hardware_concurrency());

	OutputString(L"Processor count : " + ToWString(numThreads));

	if (test == L"CACHER")
	{
		OutputString(L"Cacher-only test");
		prepareCacherTest(path);
	}
	else if (test == L"FOLDER")
	{
		OutputString(L"Folder-only test");
		prepareFolderTest(path);
	}
	else if (test == L"FULL")
	{
		OutputString(L"Full test");
		prepareCacherTest(path);
		prepareFolderTest(path);
	}

	OutputString(L"Starting threads, press Enter to quit ...");
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

void SignalError(const std::wstring& err) {
	OutputString(err, true);
	g_runner->Stop();
	g_run = false;
}
