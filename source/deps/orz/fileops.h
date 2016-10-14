#ifndef ORZ_FILEOPS_H
#define ORZ_FILEOPS_H

#include <wx/window.h>

#include <string>

namespace IO {
	bool DoFileExist(const std::string& files);
	bool DoPathExist(const std::string& file);

	bool FileDelete(const std::string& file, wxWindow* parent);
	bool FileRecycle(const std::string& file, wxWindow* parent);
	std::string Rename(const std::string& old_name, const std::string& new_name, wxWindow* parent);

	bool SupportRecycle();

	std::string GetPath(const std::string& s);
	std::string GetFile(const std::string& s);
	std::string GetTitle(const std::string& s);
	std::string GetExtension(const std::string& s);
}

#endif
