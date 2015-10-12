#include <UnitTest++/UnitTest++.h>
#include "main.h"
#include "illa/codecmgr.h"
#include "illa/surface.h"
#include "orz/fileops.h"
#include "illa/image.h"
#include "illa/codec.h"
#include <boost/scoped_array.hpp>

SUITE(CodecTests) {
	Img::CodecFactoryStore g_cfs;

	std::vector<std::string> ParseCsv(std::string filename) {
		std::vector<std::string> strings;
		IO::FileReader r(filename);
		if (!r.Open()) {
			throw Err::CriticalError("Could not open file " + filename);
		}
		std::wstring curr;
		
		char c;
		while(r.Read(&c, sizeof(c), 1)) {
			if(c == L',' || c == 10 || c == 13) {
				if(curr.empty() == false)
					strings.push_back(WStringToUTF8(curr));

				curr.clear();
			}
			else if (c != L' ')
				curr.push_back(c);

			if(curr == L"//") {
				while(c != 10 && c != 13 && r.Read(&c, sizeof(c), 1));
				curr.clear();
			}
		}
		if(curr.empty() == false)
			strings.push_back(WStringToUTF8(curr));
		return strings;
	}

	enum Error {
		ErrFileNotFound,
		ErrLoadFailed,
		ErrSizeDiffer,
		ErrDataDiffer,
		ErrNoError
	};

	struct Loaded {
		Img::Surface::Ptr ptr;
		Error err;
	};

	Loaded load(Img::AbstractCodec* pCodec, const std::string filename) {
		Loaded l = {Img::Surface::Ptr(), ErrNoError };

		IO::FileReader::Ptr f(new IO::FileReader(filename));
		//f->Filename(filename);
		if (!f->Open()) {
			DO_THROW(Err::CriticalError, "Could not open file " + filename);
		}

		if(f->IsOpen() == false) {
			l.err = ErrFileNotFound;
			return l;
		}
		l.err = ErrLoadFailed;
		if(pCodec->LoadHeader(f) == false) return l;
		if(pCodec->Allocate() != Img::AbstractCodec::AllocationStatus::Ok) return l;
		pCodec->LoadImageData();
		l.err = ErrNoError;

		l.ptr = pCodec->RequestImageComposer()->RequestCurrentSurface();
		return l;
	}

	bool isAlphaIsh(Img::Surface::Ptr p) {
		return Img::HasAlpha(p->GetFormat());
	}

	bool isIndexed(Img::Surface::Ptr p) {
		return p->GetFormat() == Img::Format::Index8;
	}


	void clearTransparent(uint32_t* bufDWord, int dwordsToCheck) {
		for(int i = 0; i < dwordsToCheck; ++i)
			if(Img::ChannelARGB8888Alpha(bufDWord[i]) == 0)
				bufDWord[i] = 0;
	}


	Error CompareImages(std::string f1, std::string f2) {
		Loaded l1 = load(g_cfs.CreateCodec(IO::GetExtension(f1)), f1);
		if(l1.err != ErrNoError) return l1.err;
		Loaded l2 = load(g_cfs.CreateCodec(IO::GetExtension(f2)), f2);
		if(l2.err != ErrNoError) return l2.err;

		if(l1.ptr->GetSize() != l2.ptr->GetSize()) return ErrSizeDiffer;

		bool isOneAlpha = isAlphaIsh(l1.ptr) || isAlphaIsh(l2.ptr);
		bool isOneNotAlpha = !isAlphaIsh(l1.ptr) || !isAlphaIsh(l2.ptr);
		if(!isIndexed(l1.ptr) && !isIndexed(l2.ptr) && isOneAlpha == isOneNotAlpha) return ErrDataDiffer;

		// Convert to ARGB32
		boost::scoped_array<uint8_t> d1(new uint8_t[l1.ptr->Width() * l1.ptr->Height() * 4]);
		boost::scoped_array<uint8_t> d2(new uint8_t[l1.ptr->Width() * l1.ptr->Height() * 4]);

		l1.ptr->CopyRectToBuffer(d1.get(), l1.ptr->Width() * 4, Geom::RectInt(Geom::PointInt(0, 0), l1.ptr->GetSize()), Img::Color(0xff, 0, 0, 0), 0, 10, 10, false);
		l2.ptr->CopyRectToBuffer(d2.get(), l1.ptr->Width() * 4, Geom::RectInt(Geom::PointInt(0, 0), l1.ptr->GetSize()), Img::Color(0xff, 0, 0, 0), 0, 10, 10, false);

		// Clear any fully transparent pixels
		int dwordsToCheck = Geom::Area(l1.ptr->GetSize());
		clearTransparent(reinterpret_cast<uint32_t*>(d1.get()), dwordsToCheck);
		clearTransparent(reinterpret_cast<uint32_t*>(d2.get()), dwordsToCheck);

		// Compare data
		int bytesToCompare = Geom::Area(l1.ptr->GetSize()) * 4;
		uint8_t* bufRaw1 = d1.get();
		uint8_t* bufRaw2 = d2.get();
		for(int i = 0; i < bytesToCompare; ++i)
			if(bufRaw1[i] != bufRaw2[i])
				return ErrDataDiffer;

		return ErrNoError;
	}

	bool ShouldFail(const std::string& file) {
		try {
			Loaded l1 = load(g_cfs.CreateCodec(IO::GetExtension(file)), file);
			if(l1.err == ErrFileNotFound) return false;
			if(l1.err == ErrNoError) return false;
		}
		catch (Err::Exception) { return true; }
		return true;
	}


	TEST(TestLoadCompare) {
		g_cfs.AddBuiltinCodecs();

		auto files = ParseCsv(g_datapath + "\\loadcompare.txt");
		for(auto i = files.begin(); i != files.end(); ++i) {
			auto a = *i;
			try {
				if(a[0] == L'!') {
					a = a.substr(1, std::string::npos);
					if(ShouldFail(g_datapath + "\\Codecs\\" + a) == false) {
						std::string wmsg = "Didn't report failure as expected: " + a;
						std::string msg(wmsg.begin(), wmsg.end());
						DECORATIVE_FAIL(msg.c_str());
					}
					continue;
				}
				else if(a[0] == L'?') {
					a = a.substr(1, std::string::npos);
					if(ShouldFail(g_datapath + "\\Codecs\\" + a)) {
						std::string wmsg = "Didn't succeed as expected: " + a;
						std::string msg(wmsg.begin(), wmsg.end());
						DECORATIVE_FAIL(msg.c_str());
					}
					continue;
				}
				i++;

				if(i == files.end())
					CHECK(false);
				auto b = *i;
				switch(CompareImages(g_datapath + "\\Codecs\\" + a, g_datapath + "\\Codecs\\" + b)) {
					case ErrNoError:
						break;
					case ErrFileNotFound:
						{
							DECORATIVE_FAIL(("File not found: " + a + " <=> " + b).c_str());
							break;
						}
					case ErrSizeDiffer:
						{
							DECORATIVE_FAIL(("Dimensions differ: " + a + " <=> " + b).c_str());
							break;
						}
					case ErrLoadFailed:
						{
							DECORATIVE_FAIL(("Failed to load: " + a + " <=> " + b).c_str());
							break;
						}

					case ErrDataDiffer:
						{
							DECORATIVE_FAIL(("Mismatch: " + a + " <=> " + b).c_str());
							break;
						}
				}
			}
			catch(Err::Exception& e) {
				DECORATIVE_FAIL(e.what());
			}
		}
	}
}
