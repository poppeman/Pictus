#include "StdAfx.h"

std::wstring g_datapath;

int wmain(int argc, wchar_t* argv[])
{
	if(argc<2) return 1;
	g_datapath=argv[1];

	return UnitTest::RunAllTests();
}
