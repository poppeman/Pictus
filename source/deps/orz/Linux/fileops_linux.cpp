#include "../fileops.h"
#include "../file_reader.h"

#include <boost/filesystem.hpp>
#include <mutex>
#include <gio/gio.h>
#include <wx/msgdlg.h>

namespace IO {
	using std::mutex;

	mutex g_mutexSHFileOperation;

	bool FileDelete(const std::string& file, wxWindow* hwnd) {
		//return performDeleteRecycle(file, false, hwnd);
		return false;
	}

	bool FileRecycle(const std::string& file, wxWindow* parent) {
		auto gf = g_file_new_for_path(file.c_str());
		GError* err = nullptr;
		auto ok = g_file_trash(gf, nullptr, &err);
		if(!ok)
		{
			wxMessageBox(wxString::FromUTF8(err->message), L"Recycle Error", wxOK);
		}
		g_error_free(err);
		g_object_unref(gf);
		return ok;
	}

	std::string Rename(const std::string& old_name, const std::string& new_name, wxWindow* hwnd)
	{
		return "";
	}

	bool SupportRecycle() {
		return true;
	}

	std::string GetPath(const std::string& s)
	{
		return boost::filesystem::path(s).make_preferred().parent_path().string() + "/";
	}
}
