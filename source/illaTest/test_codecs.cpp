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

	std::vector<std::wstring> ParseCsv(std::wstring filename) {
		std::vector<std::wstring> strings;
		IO::FileReader r(filename);
		if (!r.Open()) {
			throw Err::CriticalError(L"Could not open file " + filename);
		}
		std::wstring curr;
		
		char c;
		while(r.Read(&c, sizeof(c), 1)) {
			if(c == TX(',') || c == 10 || c == 13) {
				if(curr.empty() == false)
					strings.push_back(curr);

				curr.clear();
			}
			else if (c != TX(' '))
				curr.push_back(c);

			if(curr == L"//") {
				while(c != 10 && c != 13 && r.Read(&c, sizeof(c), 1));
				curr.clear();
			}
		}
		if(curr.empty() == false)
			strings.push_back(curr);
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

	Loaded load(Img::AbstractCodec* pCodec, const std::wstring filename) {
		Loaded l = {Img::Surface::Ptr(), ErrNoError };

		IO::FileReader::Ptr f(new IO::FileReader(filename));
		//f->Filename(filename);
		if (!f->Open()) {
			DO_THROW(Err::CriticalError, L"Could not open file " + filename);
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


	Error CompareImages(std::wstring f1, std::wstring f2) {
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

	bool ShouldFail(const std::wstring& file) {
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

		auto files = ParseCsv(g_datapath + L"\\loadcompare.txt");
		for(auto i = files.begin(); i != files.end(); ++i) {
			std::wstring a = *i;
			try {
				if(a[0] == TX('!')) {
					a = a.substr(1, std::wstring::npos);
					if(ShouldFail(g_datapath + L"\\Codecs\\" + a) == false) {
						std::wstring wmsg = L"Didn't report failure as expected: " + a;
						std::string msg(wmsg.begin(), wmsg.end());
						DECORATIVE_FAIL(msg.c_str());
					}
					continue;
				}
				else if(a[0] == TX('?')) {
					a = a.substr(1, std::wstring::npos);
					if(ShouldFail(g_datapath + L"\\Codecs\\" + a)) {
						std::wstring wmsg = L"Didn't succeed as expected: " + a;
						std::string msg(wmsg.begin(), wmsg.end());
						DECORATIVE_FAIL(msg.c_str());
					}
					continue;
				}
				i++;

				if(i == files.end())
					CHECK(false);
				std::wstring b = *i;
				switch(CompareImages(g_datapath + L"\\Codecs\\" + a, g_datapath + L"\\Codecs\\" + b)) {
					case ErrNoError:
						break;
					case ErrFileNotFound:
						{
							DECORATIVE_FAIL(ToAString(L"File not found: " + a + L" <=> " + b).c_str());
							break;
						}
					case ErrSizeDiffer:
						{
							DECORATIVE_FAIL(ToAString(L"Dimensions differ: " + a + L" <=> " + b).c_str());
							break;
						}
					case ErrLoadFailed:
						{
							DECORATIVE_FAIL(ToAString(L"Failed to load: " + a + L" <=> " + b).c_str());
							break;
						}

					case ErrDataDiffer:
						{
							DECORATIVE_FAIL(ToAString(L"Mismatch: " + a + L" <=> " + b).c_str());
							break;
						}
				}
			}
			catch(Err::Exception& e) {
				DECORATIVE_FAIL(ToAString(e.Desc()).c_str());

			}
		}
	}
}
