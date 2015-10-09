#include "fileops.h"
#include "file_reader.h"

#include <mutex>

#include <shellapi.h>

namespace IO {
	using std::mutex;

	mutex g_mutexSHFileOperation;

	struct HANDLETOMAPPINGS {
		UINT              uNumberOfMappings;  // Number of mappings in the array.
		LPSHNAMEMAPPINGW  lpSHNameMapping;    // Pointer to the array of mappings.
	};

	bool DoFileExist(const std::wstring& file) {
		if (file==L"") return false;
		IO::FileReader r(file);
		return r.Open();

		/*FILE* f = 0;
		if ((_wfopen_s(&f, file.c_str(), L"rb") == 0) && (f != 0)) {
			fclose(f);
			return true;
		}
		return false;*/
	}

	bool DoPathExist(const std::wstring& file) {
		auto dwAttr = GetFileAttributesW(file.c_str());

		if (dwAttr == INVALID_FILE_ATTRIBUTES) {
			return false;
		}

		return !!(dwAttr & FILE_ATTRIBUTE_DIRECTORY);
	}

	static std::vector<wchar_t> ToTerminatedcharArray(const std::wstring& in) {
		size_t len = in.length() + 2; // Two extra due to _two_ terminating chars.
		std::vector<wchar_t> pFromWString(len);

		// Copy source-file into the struct and apply the extra terminator.
		errno_t copyRet = wcscpy_s(&pFromWString[0], len, in.c_str());
		if (copyRet != 0) DO_THROW(Err::CriticalError, L"Couldn't copy string to buffer.");

		pFromWString[len - 1] = 0;	

		return pFromWString;
	}

	bool performDeleteRecycle(const std::wstring& file, bool doRecycle, HWND hwnd) {
		//COND_STRICT(hwnd != 0, Err::InvalidParam, L"hwnd was null.");
		SHFILEOPSTRUCTW sfop;
		ZeroMemory(&sfop, sizeof(sfop));

		auto pFromWString = ToTerminatedcharArray(file);
		sfop.pFrom = &pFromWString[0];
		sfop.wFunc = FO_DELETE;
		sfop.fFlags = FOF_SILENT;

		if(hwnd == 0) {
			sfop.fFlags |= FOF_NOCONFIRMATION;
		}

		if (doRecycle) {
			sfop.fFlags |= FOF_ALLOWUNDO;
		}

		sfop.hwnd = hwnd;

		std::lock_guard<std::mutex> l(g_mutexSHFileOperation);
		return (SHFileOperationW(&sfop) == 0) && (sfop.fAnyOperationsAborted == false);
	}

	bool FileDelete(const std::wstring& file, HWND hwnd) {
		return performDeleteRecycle(file, false, hwnd);
	}

	bool FileRecycle(const std::wstring& file, HWND hwnd) {
		return performDeleteRecycle(file, true, hwnd);
	}

	std::wstring Rename(const std::wstring& old_name, const std::wstring& new_name, HWND hwnd) {
		SHFILEOPSTRUCTW sfop;
		ZeroMemory(&sfop, sizeof(sfop));

		auto pOldName = ToTerminatedcharArray(old_name);
		auto pNewName = ToTerminatedcharArray(new_name);
		sfop.pFrom = &pOldName[0];
		sfop.pTo = &pNewName[0];
		sfop.wFunc = FO_RENAME;
		sfop.fFlags = FOF_SILENT | FOF_ALLOWUNDO | FOF_WANTMAPPINGHANDLE;
		sfop.hwnd = hwnd;

		if(hwnd == 0)
			sfop.fFlags |= FOF_NOCONFIRMATION;

		//		int ticket = pause_folders(GetPath(old_name));

		bool toRet;
		{
			std::lock_guard<std::mutex> l(g_mutexSHFileOperation);
			toRet = (SHFileOperationW(&sfop) == 0) && (sfop.fAnyOperationsAborted == false);
		}

		std::wstring resulting_name = new_name;

		HANDLETOMAPPINGS* s = reinterpret_cast<HANDLETOMAPPINGS*>(sfop.hNameMappings);
		if(s && s->uNumberOfMappings == 1) {
			resulting_name = s->lpSHNameMapping[0].pszNewPath;
		}

		SHFreeNameMappings(sfop.hNameMappings);

		/*if (toRet)
			renamed_file(old_name, resulting_name);

		restore_folders(ticket);*/
		if(!toRet) return L"";
		return resulting_name;
	}

	bool SupportRecycle() {
		return true;
	}
}
