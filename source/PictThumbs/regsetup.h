#ifndef PICTTHUMBS_REGSETUP_H
#define PICTTHUMBS_REGSETUP_H

#include "illa/codecmgr.h"

#include <string>

#include <windows.h>

HRESULT RegisterInprocServer(const std::string& clsid, const std::string& friendlyName);
HRESULT RegisterThumbnailProvider(const std::string& clsId, const std::string& extension);

HRESULT UnregisterInprocServer(const std::string& clsid);
HRESULT UnRegisterThumbnailProvider(const std::string& clsid, const std::string& extension);

#endif