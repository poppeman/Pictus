#ifndef ORZ_FILEOPS_H
#define ORZ_FILEOPS_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>

namespace IO {
	bool DoFileExist(const std::string& files);
	bool DoPathExist(const std::string& file);

#ifdef _WIN32
	bool FileDelete(const std::string& file, HWND hwnd);
	bool FileRecycle(const std::string& file, HWND hwnd);
	std::string Rename(const std::string& old_name, const std::string& new_name, HWND hwnd);
#endif

	bool SupportRecycle();

	std::string GetPath(const std::string& s);
	std::string GetParentPath(const std::string& s);
	std::string GetFile(const std::string& s);
	std::string GetTitle(const std::string& s);
	std::string GetExtension(const std::string& s);
}

#endif
