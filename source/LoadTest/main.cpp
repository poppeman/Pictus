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


Img::AbstractCodec* DetectAndLoadHeader(const std::shared_ptr<IO::FileReader> reader, std::string ext, bool autoDetect, Img::CodecFactoryStore& g_cfs) {
	if (ext.empty())
	{
		ext = IO::GetExtension(reader->Name());
	}

	auto c = g_cfs.CreateCodec(ext);
	if (c != nullptr && c->LoadHeader(reader))
	{
		return c;
	}
	else
	{
		delete c;
	}

	if(autoDetect == false)
	{
		return nullptr;
	}

	const Img::CodecFactoryStore::InfoVector& iv = g_cfs.CodecInfo();
	for (size_t i = 0; i < iv.size(); ++i)
	{
		c = g_cfs.CreateCodec(i);
		if (c == nullptr)
		{
			continue;
		}

		reader->Seek(0, IO::SeekMethod::Begin);
		if (!c->CanDetectFormat() || !c->LoadHeader(reader))
		{
			delete c;
		}
		else
		{
			return c;
		}
	}
	return nullptr;
}

int performLoad(const std::string& filename, const std::string& ext, bool autoDetect, Img::CodecFactoryStore& g_cfs, size_t max_size)
{
	auto f = std::make_shared<IO::FileReader>(filename);
	if(!f->Open())
	{
		std::cout << "Failed opening file" << std::endl;
		return EXIT_FAILURE;
	}
	auto pCodec = DetectAndLoadHeader(f, ext, autoDetect, g_cfs);
	if(pCodec == nullptr)
	{
		std::cout << "No codec!" << std::endl;
		return EXIT_FAILURE;
	}
	if(max_size > 0)
	{
		auto sz = pCodec->GetSize();
		auto ps = Img::EstimatePixelSize(pCodec->GetFormat());
		if(static_cast<size_t>(sz.Width * sz.Height) * ps > max_size) {
			std::cout << "Image would become too large!" << std::endl;
			delete pCodec;
			return EXIT_FAILURE;
		}
	}

	if(pCodec->Allocate() != Img::AbstractCodec::AllocationStatus::Ok)
	{
		std::cout << "Failed allocating surface!" << std::endl;
		delete pCodec;
		return EXIT_FAILURE;
	}
	pCodec->LoadImageData();
	delete pCodec;
	return EXIT_SUCCESS;
}

int realMain(const std::vector<std::string>& args) {
	Img::CodecFactoryStore g_cfs;

	if(args.empty())
	{
		std::cout << "Missing parameter" << std::endl;
		return EXIT_FAILURE;
	}

	std::locale::global(boost::locale::generator().generate(""));
	boost::filesystem::path::imbue(std::locale());

	g_cfs.AddBuiltinCodecs();

	bool autoDetect = true;
	bool bench = false;
	std::string filename = "";

	std::string ext = "";
	size_t max_size = 0;

	for(auto& p:args)
	{
		if(p == "--noauto")
		{
			autoDetect = false;
		}
		else if (p.find("--ext=") == 0 && p.length() > 6)
		{
			autoDetect = false;
			ext = p.substr(6);
		}
		else if(p == "--bench")
		{
			bench = true;
		}
		else if(p.find("--max-size=") == 0 && p.length() > 11)
		{
			std::stringstream ss;
			ss << p.substr(11, std::string::npos);
			ss >> max_size;
		}
		else
		{
			filename = p;
		}
	}

	if(filename.empty())
	{
		std::cout << "Missing filename param" << std::endl;
		return EXIT_FAILURE;
	}

	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);

	if(bench)
	{
		std::cout << "Benchmarking..." << std::endl;
		Util::StopWatch sw;

		for (int i = 0; i < Warmups; ++i)
		{
			if (performLoad(filename, ext, autoDetect, g_cfs, max_size) == EXIT_FAILURE)
			{
				std::cout << "Failed during warmup, exiting ..." << std::endl;
				return EXIT_FAILURE;
			}
		}

		sw.Start();
		for (int i = 0; i < NumRuns; ++i)
		{
			performLoad(filename, ext, autoDetect, g_cfs, max_size);
		}
		int time = sw.Stop();

		std::cout << "Time: " << (time / NumRuns) << std::endl;
	}
	else
	{
		try {
#ifdef __AFL_HAVE_MANUAL_CONTROL
			__AFL_INIT();
#endif
			return performLoad(filename, ext, autoDetect, g_cfs, max_size);
		}
		catch (std::bad_alloc &e) {
			std::cout << "Ran out of memory, that's no good!\n";
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[])
{
	std::vector<std::string> args;
	for(auto i = 1; i < argc; i++)
	{
		args.push_back(WStringToUTF8(argv[i]));
	}

	return realMain(args);
}
#else
int main(int argc, char* argv[])
{
	std::vector<std::string> args;
	for(auto i = 1; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	return realMain(args);
}
#endif
