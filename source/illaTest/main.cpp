#include <UnitTest++/UnitTest++.h>

#include "illa/surfacemgr.h"
#include "illa/swsurface.h"

std::string g_datapath;

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[]) {
	if(argc < 2) return 1;
	g_datapath = WStringToUTF8(argv[1]);

	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);

	return UnitTest::RunAllTests();
}
#else
int main(int argc, char* argv[]) {
	if(argc < 2) return 1;
	g_datapath = argv[1];

	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);

	return UnitTest::RunAllTests();
}
#endif
