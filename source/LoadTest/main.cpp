#include "illa/codec.h"
#include "illa/codecmgr.h"
#include "illa/surfacemgr.h"
#include "illa/swsurface.h"
#include "orz/fileops.h"
#include "orz/logger.h"
#include "orz/stopwatch.h"

enum
{
	Warmups = 1,
	NumRuns = 3
};

Img::CodecFactoryStore g_cfs;

int performLoad(const std::string& filename)
{
	Img::AbstractCodec::Ptr pCodec(g_cfs.CreateCodec(IO::GetExtension(filename)));
	auto f = std::make_shared<IO::FileReader>(filename);
	//f->Filename(filename);
	f->Open();
	if(pCodec->LoadHeader(f) == false) return EXIT_FAILURE;
	if(pCodec->Allocate() != Img::AbstractCodec::AllocationStatus::Ok) return EXIT_FAILURE;
	pCodec->LoadImageData();
	return EXIT_SUCCESS;
}

int realMain(std::string filename) {
	g_cfs.AddBuiltinCodecs();

	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);

	Util::StopWatch sw;

	for (int i = 0; i < Warmups; ++i)
		if (performLoad(filename) == EXIT_FAILURE) return EXIT_FAILURE;

	sw.Start();
	for (int i = 0; i < NumRuns; ++i)
		performLoad(filename);
	int time = sw.Stop();

	Log << "Time: " << (time / NumRuns) << "\n";

	return EXIT_SUCCESS;	
}

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[])
{
	if (argc < 2) {
		std::cout << "Missing parameter" << std::endl;
		return EXIT_FAILURE;
	}
	return realMain(WStringToUTF8(argv[1]));
}
#else
int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "Missing parameter" << std::endl;
		return EXIT_FAILURE;
	}
	return realMain(argv[1]);
}
#endif
