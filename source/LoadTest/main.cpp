#include "illa/codec.h"
#include "illa/codecmgr.h"
#include "illa/surfacemgr.h"
#include "illa/swsurface.h"
#include "orz/fileops.h"
#include "orz/logger.h"
#include "orz/stopwatch.h"

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

enum
{
	Warmups = 1,
	NumRuns = 3
};

Img::CodecFactoryStore g_cfs;

Img::AbstractCodec* DetectAndLoadHeader(const std::shared_ptr<IO::FileReader> reader) {
	auto c = g_cfs.CreateCodec(IO::GetExtension(reader->Name()));
	if (c != nullptr && c->LoadHeader(reader)) {
		return c;
	}

	const Img::CodecFactoryStore::InfoVector& iv = g_cfs.CodecInfo();
	for (size_t i = 0; i < iv.size(); ++i) {
		c = g_cfs.CreateCodec(i);
		if (c == nullptr) {
			continue;
		}

		reader->Seek(0, IO::SeekMethod::Begin);
		if (!c->CanDetectFormat() || !c->LoadHeader(reader)) {
			delete c;
		}
		else {
			return c;
		}
	}
	return 0;
}

int performLoad(const std::string& filename)
{
	auto f = std::make_shared<IO::FileReader>(filename);
	if(!f->Open()) {
		std::cout << "Failed opening file" << std::endl;
		return EXIT_FAILURE;
	}
	auto pCodec = DetectAndLoadHeader(f);
	if(pCodec == nullptr) {
		std::cout << "No codec!" << std::endl;
		return EXIT_FAILURE;
	}

	if(pCodec->LoadHeader(f) == false) return EXIT_FAILURE;
	if(pCodec->Allocate() != Img::AbstractCodec::AllocationStatus::Ok) return EXIT_FAILURE;
	pCodec->LoadImageData();
	return EXIT_SUCCESS;
}

int realMain(std::string filename) {
	std::locale::global(boost::locale::generator().generate(""));
	boost::filesystem::path::imbue(std::locale());

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
	realMain(argv[1]);
	return EXIT_SUCCESS;
}
#endif
