#include "StdAfx.h"

#include "folder_imp.h"
#include "folder.h"

//#pragma comment(lib, "shlwapi.lib")

namespace IO {
/*	bool FolderImplementation::Path(const std::wstring& path) {
		m_path = path;
		return onSetPath();
	}

	FileList FolderImplementation::CurrentContents() {
		return onCurrentContents();
	}

	std::wstring& FolderImplementation::Path() {
		return m_path;
	}

	void FolderImplementation::OnActivity() {
		Parent()->OnUpdate();
	}

	void FolderImplementation::OnFolderDeleted() {
		Parent()->OnDeleted();
	}

	FolderImplementation::FolderImplementation()
		:m_isMonitoring(false),
		 m_parent(0)
	{}
	FolderImplementation::~FolderImplementation() {}

	void FolderImplementation::StartMonitoring(Folder* const parent) {
		COND_STRICT(m_isMonitoring == false, Err::CriticalError, TX("Shouldn't be monitoring now."));
		m_parent = parent;
		m_isMonitoring = true;
		onStartMonitoring();
	}
	void FolderImplementation::StopMonitoring() {
		COND_STRICT(m_isMonitoring, Err::CriticalError, TX("Should be monitoring now."));
		onStopMonitoring();
	}

	Folder* FolderImplementation::Parent() {
		return m_parent;
	}
	*/

	const std::wstring& FolderImplementation::Path() const {
		return m_path;
	}

	FolderImplementation::FolderImplementation(const std::wstring& path):m_path(path) {
	}
}
