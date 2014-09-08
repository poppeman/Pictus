#include "StdAfx.h"
#include "regsetup.h"
#include "codecsetup.h"
#include "dllmain.h"
#include "regutils.h"

HRESULT RegisterInprocServer(const std::wstring& clsid, const std::wstring& friendlyName) {
	WCHAR szModuleName[MAX_PATH];

	if (!GetModuleFileNameW(g_hInst, szModuleName, ARRAYSIZE(szModuleName))) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	auto subKey = std::wstring(L"CLSID\\") + clsid;
	auto servKey = subKey + L"\\InProcServer32";
	HRESULT hr;
	hr = SetHkcrRegistryKeyAndValue(subKey, 0, friendlyName.c_str());
	if (FAILED(hr)) return hr;

	hr = SetHkcrRegistryKeyAndValue(servKey, 0, szModuleName);
	if (FAILED(hr)) return hr;

	hr = SetHkcrRegistryKeyAndValue(servKey, L"ThreadingModel", L"Apartment");
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT RegisterThumbnailProvider(const std::wstring& clsId, const std::wstring& extension) {
	// We always register the shellex directly into the extension key.
	// This allows thumbnails to work for that format even when the user changes associations (progids).

	auto currentSubKey = L"." + extension + L"\\shellex\\{e357fccd-a995-4576-b01f-234630154e96}";
	return SetHkcrRegistryKeyAndValue(currentSubKey, 0, clsId.c_str());
}


HRESULT UnregisterInprocServer(const std::wstring& clsid) {
	auto subKey = L"CLSID\\" + clsid;

	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, subKey.c_str()));
}

HRESULT UnRegisterThumbnailProvider(const std::wstring& clsId, const std::wstring& extension) {
	// TODO: Verify that we are the current shellex for this extension!
	auto currentSubKey = L"." + extension + L"\\shellex\\{e357fccd-a995-4576-b01f-234630154e96}";

	auto ret = GetHkcrRegistryKeyAndValue(currentSubKey, 0);
	auto hr = std::get<0>(ret);
	auto currentClsId = std::get<1>(ret);
	if (FAILED(hr)) return hr; // Couldn't read value, that's an error

	if (currentClsId != clsId) return S_OK; // Clsids didn't match. Not an error, but we shouldn't remove the key.

	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, currentSubKey.c_str()));
}
