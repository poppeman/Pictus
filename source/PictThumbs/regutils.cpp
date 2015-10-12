#include "regutils.h"

#include "orz/logger.h"
#include "orz/types.h"

#include <tuple>
#include <vector>

HRESULT SetHkcrRegistryKeyAndValue(const std::string& subKey, const char* valueName, const char* data) {
	HKEY hKey = 0;

	auto hr = HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_CLASSES_ROOT, UTF8ToWString(subKey).c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey, 0));

	if (FAILED(hr)) return hr;

	if (data != 0)
	{
		auto wideData = UTF8ToWString(data);

		// Set the specified value of the key.
		DWORD cbData = wideData.length() * sizeof(wchar_t);
		hr = HRESULT_FROM_WIN32(RegSetValueEx(hKey, UTF8ToWString(valueName).c_str(), 0, REG_SZ, reinterpret_cast<const BYTE *>(&wideData[0]), cbData));
	}

	RegCloseKey(hKey);

	return hr;
}

std::tuple<HRESULT, std::string> GetHkcrRegistryKeyAndValue(const std::string& subKey, const char* valueName) {
	const int MaxValueLength = 256;

		
	HKEY hKey = NULL;
	auto hr = HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CLASSES_ROOT, UTF8ToWString(subKey).c_str(), 0, KEY_READ, &hKey));

	std::string data;
	if (SUCCEEDED(hr)) {
		std::vector<wchar_t> tmpData(MaxValueLength);
		DWORD dataLength = MaxValueLength * sizeof(wchar_t);

		hr = HRESULT_FROM_WIN32(RegQueryValueEx(
			hKey,
			UTF8ToWString(valueName).c_str(),
			0,
			0,
			reinterpret_cast<LPBYTE>(&tmpData[0]),
			&dataLength));

		if (SUCCEEDED(hr) && dataLength > 2) {
			data = WStringToUTF8(std::wstring(tmpData.begin(), tmpData.begin() + dataLength / sizeof(wchar_t) - 1));
		}
		else {
			Log << "(Thumbs:GetHkcrRegistryKeyAndValue): Failed querying value for " << subKey << "\n";
		}

		RegCloseKey(hKey);
	}

	return std::tuple<HRESULT, std::string>(hr, data);
}
