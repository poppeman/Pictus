#include "regutils.h"

#include "orz/logger.h"

#include <tuple>
#include <vector>

HRESULT SetHkcrRegistryKeyAndValue(const std::wstring& subKey, const wchar_t* valueName, const wchar_t* data) {
	HKEY hKey = 0;

	auto hr = HRESULT_FROM_WIN32(RegCreateKeyEx(HKEY_CLASSES_ROOT, subKey.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey, 0));

	if (FAILED(hr)) return hr;

	if (data != 0)
	{
		// Set the specified value of the key.
		DWORD cbData = lstrlenW(data) * sizeof(wchar_t);
		hr = HRESULT_FROM_WIN32(RegSetValueEx(hKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE *>(data), cbData));
	}

	RegCloseKey(hKey);

	return hr;
}

std::tuple<HRESULT, std::wstring> GetHkcrRegistryKeyAndValue(const std::wstring& subKey, const wchar_t* valueName) {
	const int MaxValueLength = 256;

		
	HKEY hKey = NULL;
	auto hr = HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CLASSES_ROOT, subKey.c_str(), 0, KEY_READ, &hKey));

	std::wstring data;
	if (SUCCEEDED(hr)) {
		std::vector<wchar_t> tmpData(MaxValueLength);
		DWORD dataLength = MaxValueLength * sizeof(wchar_t);

		hr = HRESULT_FROM_WIN32(RegQueryValueEx(
			hKey,
			valueName,
			0,
			0,
			reinterpret_cast<LPBYTE>(&tmpData[0]),
			&dataLength));

		if (SUCCEEDED(hr) && dataLength > 2) {
			data = std::wstring(tmpData.begin(), tmpData.begin() + dataLength / sizeof(wchar_t) - 1);
		}
		else {
			Log << L"(Thumbs:GetHkcrRegistryKeyAndValue): Failed querying value for " << subKey << L"\n";
		}

		RegCloseKey(hKey);
	}

	return std::tuple<HRESULT, std::wstring>(hr, data);
}


HRESULT GetHKCRRegistryKeyAndValue(PCWSTR pszSubKey, PCWSTR pszValueName,
	PWSTR pszData, DWORD cbData)
{
	HRESULT hr;
	HKEY hKey = NULL;

	// Try to open the specified registry key. 
	hr = HRESULT_FROM_WIN32(RegOpenKeyEx(HKEY_CLASSES_ROOT, pszSubKey, 0,
		KEY_READ, &hKey));

	if (SUCCEEDED(hr))
	{
		// Get the data for the specified value name.
		hr = HRESULT_FROM_WIN32(RegQueryValueEx(hKey, pszValueName, NULL,
			NULL, reinterpret_cast<LPBYTE>(pszData), &cbData));

		RegCloseKey(hKey);
	}

	return hr;
}
