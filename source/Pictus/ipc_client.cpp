#include "ipc_client.h"
#include "ipc_openfile_connection.h"

namespace IPC
{
	wxConnectionBase * OpenFileClient::OnMakeConnection()
	{
		m_connection = std::make_shared<OpenFileConnection>();
		return m_connection.get();
	}
}
