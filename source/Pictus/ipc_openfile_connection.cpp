#include "ipc_openfile_connection.h"
#include "orz/types.h"

namespace IPC
{
	bool OpenFileConnection::OnPoke(const wxString & topic, const wxString & item, const void * data, size_t size, wxIPCFormat format)
	{
		const char* cData = reinterpret_cast<const char*>(data);
		std::string wstr(cData, cData + size);
		HandleOpenFile(wstr);
		return true;
	}
	void OpenFileConnection::SendOpenFile(std::string path)
	{
		if (path == "")
		{
			path = " ";
		}
		Poke(L"OPEN", path.c_str(), path.size());
	}
}
