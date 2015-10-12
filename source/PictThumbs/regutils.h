#ifndef PICTTHUMBS_REGUTILS_H
#define PICTTHUMBS_REGUTILS_H

#include <string>

#include <windows.h>

HRESULT SetHkcrRegistryKeyAndValue(const std::string& subKey, const char* valueName, const char* data);
std::tuple<HRESULT, std::string> GetHkcrRegistryKeyAndValue(const std::string& subKey, const char* valueName);

#endif
