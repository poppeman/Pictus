#ifndef PICTUS_IPC_CLIENT_H
#define PICTUS_IPC_CLIENT_H

#include <wx/ipc.h>
#include <memory>

#include "ipc_openfile_connection.h"

namespace IPC
{
	class OpenFileClient : public wxClient
	{
	public:
		wxConnectionBase* OnMakeConnection() override;

	private:
		std::shared_ptr<OpenFileConnection> m_connection;
	};
}

#endif
