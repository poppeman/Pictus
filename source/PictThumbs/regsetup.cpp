#include "regsetup.h"
#include "codecsetup.h"
#include "dllmain.h"
#include "regutils.h"
#include "orz/logger.h"

HRESULT RegisterInprocServer(const std::wstring& clsid, const std::wstring& friendlyName) {
	Log << L"(Thumb): Register InprocServer\n";
	WCHAR szModuleName[MAX_PATH];

	if (!GetModuleFileNameW(g_hInst, szModuleName, ARRAYSIZE(szModuleName))) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	auto subKey = std::wstring(L"CLSID\\") + clsid;
	auto servKey = subKey + L"\\InProcServer32";
	HRESULT hr;
	hr = SetHkcrRegistryKeyAndValue(subKey, 0, friendlyName.c_str());
	if (FAILED(hr)) {
		Log << L"(Thumb): Failed creating key/value for CLSID\n";
		return hr;
	}

	hr = SetHkcrRegistryKeyAndValue(servKey, 0, szModuleName);
	if (FAILED(hr)) {
		Log << L"(Thumb): Failed creating key/value for InProcServer32\n";
		return hr;
	}

	hr = SetHkcrRegistryKeyAndValue(servKey, L"ThreadingModel", L"Apartment");
	if (FAILED(hr)) {
		Log << L"(Thumb): Failed creating key/value for ThreadingModel\n";
		return hr;
	}

	return hr;
}

HRESULT RegisterThumbnailProvider(const std::wstring& clsId, const std::wstring& extension) {
	// We always register the shellex directly into the extension key.
	// This allows thumbnails to work for that format even when the user changes associations (progids).
	Log << L"(Thumb): Registering thumbnail provider for " << extension << L"\n";

	auto currentSubKey = L"." + extension + L"\\shellex\\{e357fccd-a995-4576-b01f-234630154e96}";
	return SetHkcrRegistryKeyAndValue(currentSubKey, 0, clsId.c_str());
}


HRESULT UnregisterInprocServer(const std::wstring& clsid) {
	Log << L"(Thumb): Unregistering InprocServer\n";
	auto subKey = L"CLSID\\" + clsid;

	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, subKey.c_str()));
}

HRESULT UnRegisterThumbnailProvider(const std::wstring& clsId, const std::wstring& extension) {
	Log << L"(Thumb): Unregistering thumbnail provider for " << extension << L"\n";

	auto currentSubKey = L"." + extension + L"\\shellex\\{e357fccd-a995-4576-b01f-234630154e96}";

	auto ret = GetHkcrRegistryKeyAndValue(currentSubKey, 0);
	auto hr = std::get<0>(ret);
	auto currentClsId = std::get<1>(ret);
	if (FAILED(hr)) return hr; // Couldn't read value, that's an error

	if (currentClsId != clsId) {
		Log << L"(Thumb:UnRegisterThumbnailProvider): CLSID mismatch for " << extension << L", expected " << clsId << L" but got " << currentClsId << L"\n";
		return S_OK; // Clsids didn't match. Not an error, but we shouldn't remove the key.
	}

	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, currentSubKey.c_str()));
}
