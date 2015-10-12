#include "regsetup.h"
#include "codecsetup.h"
#include "dllmain.h"
#include "regutils.h"
#include "orz/logger.h"

HRESULT RegisterInprocServer(const std::string& clsid, const std::string& friendlyName) {
	Log << "(Thumb): Register InprocServer\n";
	WCHAR szModuleName[MAX_PATH];

	if (!GetModuleFileNameW(g_hInst, szModuleName, ARRAYSIZE(szModuleName))) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	auto moduleName = WStringToUTF8(szModuleName);

	auto subKey = "CLSID\\" + clsid;
	auto servKey = subKey + "\\InProcServer32";
	HRESULT hr;
	hr = SetHkcrRegistryKeyAndValue(subKey, 0, friendlyName.c_str());
	if (FAILED(hr)) {
		Log << "(Thumb): Failed creating key/value for CLSID\n";
		return hr;
	}

	hr = SetHkcrRegistryKeyAndValue(servKey, 0, moduleName.c_str());
	if (FAILED(hr)) {
		Log << "(Thumb): Failed creating key/value for InProcServer32\n";
		return hr;
	}

	hr = SetHkcrRegistryKeyAndValue(servKey, "ThreadingModel", "Apartment");
	if (FAILED(hr)) {
		Log << "(Thumb): Failed creating key/value for ThreadingModel\n";
		return hr;
	}

	return hr;
}

HRESULT RegisterThumbnailProvider(const std::string& clsId, const std::string& extension) {
	// We always register the shellex directly into the extension key.
	// This allows thumbnails to work for that format even when the user changes associations (progids).
	Log << "(Thumb): Registering thumbnail provider for " << extension << "\n";

	auto currentSubKey = "." + extension + "\\shellex\\{e357fccd-a995-4576-b01f-234630154e96}";
	return SetHkcrRegistryKeyAndValue(currentSubKey, 0, clsId.c_str());
}


HRESULT UnregisterInprocServer(const std::string& clsid) {
	Log << "(Thumb): Unregistering InprocServer\n";
	auto subKey = "CLSID\\" + clsid;

	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, UTF8ToWString(subKey).c_str()));
}

HRESULT UnRegisterThumbnailProvider(const std::string& clsId, const std::string& extension) {
	Log << "(Thumb): Unregistering thumbnail provider for " << extension << "\n";

	auto currentSubKey = "." + extension + "\\shellex\\{e357fccd-a995-4576-b01f-234630154e96}";

	auto ret = GetHkcrRegistryKeyAndValue(currentSubKey, 0);
	auto hr = std::get<0>(ret);
	auto currentClsId = std::get<1>(ret);
	if (FAILED(hr)) return hr; // Couldn't read value, that's an error

	if (currentClsId != clsId) {
		Log << "(Thumb:UnRegisterThumbnailProvider): CLSID mismatch for " << extension << ", expected " << clsId << " but got " << currentClsId << "\n";
		return S_OK; // Clsids didn't match. Not an error, but we shouldn't remove the key.
	}

	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, UTF8ToWString(currentSubKey).c_str()));
}
