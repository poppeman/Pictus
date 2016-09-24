#ifndef PICTUS_IPC_SERVER_H
#define PICTUS_IPC_SERVER_H

#include <wx/ipc.h>
#include <memory>
#include "ipc_openfile_connection.h"

namespace App
{
	class Viewer;
}

namespace IPC
{
	class OpenFileServer:public wxServer
	{
	public:
		wxConnectionBase* OnAcceptConnection(const wxString &topic) override;
		OpenFileServer(App::Viewer* viewer);

	private:
		App::Viewer* m_viewer;
	};
}

#endif
