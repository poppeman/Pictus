#include "StdAfx.h"
#include "illa/codecmgr.h"
#include "illa/codec.h"
#include "orz/file.h"
#include "orz/stream_mem.h"
#include "orz/orz_math.h"
#include "illa/surfacemgr.h"
#include "illa/swsurface.h"

const int MaxRandomBytes = 100;
const int NumThreads = 2;

Img::CodecFactoryStore cfs;
Math::Randomizer r;

bool g_run = true;

FileInt BigRandom() {
	FileInt f = ((static_cast<FileInt>(r.Random()) << 32) + r.Random()) & 0x7fffffffffffffff ;
	return f;
}

void FuzzTest(uint8_t* buffer, FileInt size) {
	IO::FileReader::Ptr memReader(new IO::FileReader(IO::Stream::Ptr(new IO::StreamMemory(buffer, size))));
	memReader->Open();
	size_t numCodecs = cfs.CodecInfo().size();

	int toRandom = r.Random() % MaxRandomBytes;

	for(int i=0;i<toRandom;++i)
		buffer[BigRandom() % size] = r.Random() % 256;

	for(size_t i=0;i<numCodecs;++i) {
		memReader->Seek(0, IO::SeekMethod::Begin);
		std::auto_ptr<Img::AbstractCodec> c (cfs.CreateCodec(i)); 
		if(!c->LoadHeader(memReader)) continue;
		if(c->EstimateMemory() > 1024 * 1024 * 1024) continue;
		c->Allocate();
		c->LoadImageData();
		break;
	}
}

size_t DetermineSize(size_t max) {
	unsigned long deltaSize = (r.Random() % max) / 2;
	if(deltaSize < 80) return max;
	if(deltaSize < 90) return max - deltaSize;
	return max + deltaSize;
}

void ThreadJob(const uint8_t* sourceData, size_t size) {
	while(g_run) {
		size_t destSize = DetermineSize(size);
		boost::scoped_array<uint8_t> dupe(new uint8_t[destSize]);
		memcpy(dupe.get(), sourceData, Util::Min(destSize, size));
		FuzzTest(dupe.get(), destSize);
	}
}

int wmain(int argc, wchar_t* argv[]) {
	if(argc != 2)
		return EXIT_FAILURE;
	Img::SurfaceFactory(new Img::FactorySurfaceSoftware);
	cfs.AddBuiltinCodecs();

	IO::FileReader f(argv[1]);
	f.Open();
	boost::scoped_array<uint8_t> originalData(new uint8_t[f.Size()]);
	f.ReadFull(originalData.get(), f.Size());

	boost::thread_group threads;

	for(int i=0;i<NumThreads;++i)
		threads.create_thread([&]() { ThreadJob(originalData.get(), f.Size()); });

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
	g_run = false;
	threads.join_all();
}
