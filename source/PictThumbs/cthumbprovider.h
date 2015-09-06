#ifndef CTHUMBPROVIDER_H
#define CTHUMBPROVIDER_H

#include "illa/codec.h"
#include "illa/codecmgr.h"
#include "orz/logger.h"

class CPictusThumbnailProvider:public IInitializeWithStream, public IThumbnailProvider {
public:
	CPictusThumbnailProvider();

	virtual ~CPictusThumbnailProvider();

	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	// IInitializeWithStream
	IFACEMETHODIMP Initialize(_In_ IStream *pStream, _In_ DWORD grfMode);

	// IThumbnailProvider
	IFACEMETHODIMP GetThumbnail(_In_ UINT cx, _Deref_opt_out_ HBITMAP *phbmp, __RPC__out WTS_ALPHATYPE *pdwAlpha);

private:
	Img::Surface::Ptr LoadSurface(UINT cx);
	Img::AbstractCodec* FindCodec();

	Img::CodecFactoryStore m_cfs;

	long m_cRef;
	//IStream *m_pStream;
	IO::FileReader::Ptr m_reader;
};

HRESULT CPictusThumbnailProvider_CreateInstance(REFIID riid, __deref_out void **ppv);
#endif
