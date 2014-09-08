#include "StdAfx.h"

#include <UnitTest++/TestResults.h>

#include "illa/surfacemgr.h"
#include "illa/swsurface.h"

std::wstring g_datapath;

int wmain(int argc, wchar_t* argv[]) {
	if(argc < 2) return 1;
	g_datapath = argv[1];

	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);

	return UnitTest::RunAllTests();
}
