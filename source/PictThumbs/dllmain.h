#ifndef PICTTHUMBS_DLLMAIN_H
#define PICTTHUMBS_DLLMAIN_H

#include <windows.h>

// {36FCD09A-A906-4cd0-8EC9-52EB6E097DFB}
// TODO: Don't let these be defines.
#define SZ_CLSID_PICTTHUMBSPROVIDER		L"{36FCD09A-A906-4cd0-8EC9-52EB6E097DFB}"
#define SZ_THUMBNAILPROVIDERNAME		L"Pictus Thumbnail Provider"

static const CLSID CLSID_PictusThumbnailProvider = { 0x36fcd09a, 0xa906, 0x4cd0, { 0x8e, 0xc9, 0x52, 0xeb, 0x6e, 0x9, 0x7d, 0xfb } };
extern HMODULE g_hInst;

#endif
