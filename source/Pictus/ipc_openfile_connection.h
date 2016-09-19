#ifndef PICTUS_IPC_OPENFILE_CONNECTION_H
#define PICTUS_IPC_OPENFILE_CONNECTION_H

#include <wx/ipc.h>

namespace IPC
{
	class OpenFileConnection :public wxConnection
	{
	public:
		bool OnPoke(const wxString& topic, const wxString& item, const void* data, size_t size, wxIPCFormat format) override;

		void SendOpenFile(std::string path);

		std::function<void(std::string)> HandleOpenFile;
	};
}

#endif
