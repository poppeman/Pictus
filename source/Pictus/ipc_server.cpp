#include "ipc_server.h"
#include "ipc_openfile_connection.h"
#include "viewer.h"

namespace IPC
{
	wxConnectionBase * OpenFileServer::OnAcceptConnection(const wxString & topic)
	{
		if (topic == L"OPENFILE")
		{
			auto connection = new OpenFileConnection();
			connection->HandleOpenFile = [=](std::string destination) {
				m_viewer->SetImageLocation(destination);
			};
			return connection;
		}
		return nullptr;
	}
	OpenFileServer::OpenFileServer(App::Viewer * viewer):
		m_viewer{ viewer }
	{
	}
}
