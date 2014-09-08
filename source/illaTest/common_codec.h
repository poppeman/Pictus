
#ifndef COMMON_CODEC_H
#define COMMON_CODEC_H

#include "illa/codec.h"
#include "illa/surface.h"
#include "main.h"

inline Img::Surface::Ptr load(Img::AbstractCodec* pCodec, const std::wstring filename)
{
	IO::FileReader::Ptr f(new IO::FileReader);
	f->Filename(filename);
	f->Open();
	if(f->IsOpen() == false) return Img::Surface::Ptr();
	if(pCodec->LoadHeader(f) == false) return Img::Surface::Ptr();
	if(pCodec->Allocate() == false) return Img::Surface::Ptr();
	pCodec->LoadImageData();

	return pCodec->RequestImageComposer()->RequestCurrentSurface();

	//Img::Surface::Ptr s1 = pCodec->GetSurface();
	//return s1;
}

template <class C>
inline bool ShouldFail(const std::wstring& file)
{
	C codec;
	try
	{
		return (load(&codec, g_datapath + file) == 0);
	}
	catch(Err::Exception)
	{
		return true;
	}
}

#endif
