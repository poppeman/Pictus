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

int performLoad(const std::wstring& filename)
{
	Img::AbstractCodec::Ptr pCodec(g_cfs.CreateCodec(IO::GetExtension(filename)));
	IO::FileReader::Ptr f(new IO::FileReader(filename));
	//f->Filename(filename);
	f->Open();
	if(pCodec->LoadHeader(f) == false) return EXIT_FAILURE;
	if(pCodec->Allocate() != Img::AbstractCodec::AllocationStatus::Ok) return EXIT_FAILURE;
	pCodec->LoadImageData();
	return EXIT_SUCCESS;
}

int wmain(int argc, wchar_t* argv[])
{
	g_cfs.AddBuiltinCodecs();
	if (argc < 2) return EXIT_FAILURE;

	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);

	std::wstring filename(argv[1]);

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
