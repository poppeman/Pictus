#ifndef ORZ_FILEOPS_H
#define ORZ_FILEOPS_H

#ifdef WIN32
#include <windows.h>
#endif

#include <string>

namespace IO {
	bool DoFileExist(const std::wstring& files);
	bool DoPathExist(const std::wstring& file);

#ifdef WIN32
	bool FileDelete(const std::wstring& file, HWND hwnd);
	bool FileRecycle(const std::wstring& file, HWND hwnd);
	std::wstring Rename(const std::wstring& old_name, const std::wstring& new_name, HWND hwnd);
#endif

	bool SupportRecycle();

	std::wstring GetPath(const std::wstring& s);
	std::wstring GetParentPath(const std::wstring& s);
	std::wstring GetFile(const std::wstring& s);
	std::wstring GetTitle(const std::wstring& s);
	std::wstring GetExtension(const std::wstring& s);
}

#endif
