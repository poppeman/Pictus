#ifndef PICTTHUMBS_REGSETUP_H
#define PICTTHUMBS_REGSETUP_H

#include "illa/codecmgr.h"

#include <string>

#include <windows.h>

HRESULT RegisterInprocServer(const std::wstring& clsid, const std::wstring& friendlyName);
HRESULT RegisterThumbnailProvider(const std::wstring& clsId, const std::wstring& extension);

HRESULT UnregisterInprocServer(const std::wstring& clsid);
HRESULT UnRegisterThumbnailProvider(const std::wstring& clsid, const std::wstring& extension);

#endif